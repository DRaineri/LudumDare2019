
#include "TDVCharacter.h"

#include "UObject/ConstructorHelpers.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/SpringArmComponent.h"
#include "MyGameInstance.h"
#include "MainGame/MainGameState.h"
#include "MainGame/MainGameController.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Runtime/Engine/Classes/Kismet/KismetMathLibrary.h"
#include "Engine/World.h"

ATDVCharacter::ATDVCharacter()
{
	// Set size for player capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate character to camera direction
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Rotate character to moving direction
	GetCharacterMovement()->RotationRate = FRotator(0.f, 640.f, 0.f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	// Create a camera boom...
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->bAbsoluteRotation = true; // Don't want arm to rotate when character does
	CameraBoom->TargetArmLength = 800.f;
	CameraBoom->RelativeRotation = FRotator(-60.f, 0.f, 0.f);
	CameraBoom->bDoCollisionTest = false; // Don't want to pull camera in when it collides with level

	// Create a camera...
	TopDownCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("TopDownCamera"));
	TopDownCameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	TopDownCameraComponent->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Activate ticking in order to update the cursor every frame.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
}

void ATDVCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (IsLocallyControlled())
	{
		if (!TopDownWidget)
			TopDownWidget = CreateWidget<UUserWidget>(GetWorld(), wTopDownWidget);
		TopDownWidget->AddToViewport();
	}
}

void ATDVCharacter::Destroyed()
{
	Super::Destroyed();

	if (IsLocallyControlled())
	{
		if (TopDownWidget)
			TopDownWidget->RemoveFromParent();
	}
}

void ATDVCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	AimUsingMouseCursor();
}

void ATDVCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Bind movement events
	PlayerInputComponent->BindAxis("MoveForward", this, &ATDVCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ATDVCharacter::MoveRight);

	// Bind fire event
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &ATDVCharacter::OnFire);

	PlayerInputComponent->BindAction("InviteFriend", EInputEvent::IE_Pressed, this, &ATDVCharacter::InviteFriend);
}

void ATDVCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	APlayerController* playerController = Cast<APlayerController>(NewController);
	if (IsValid(playerController))
	{
		playerController->bShowMouseCursor = true;
	}
}


void ATDVCharacter::MoveForward(float val)
{
	if (val != 0.0f)
	{
		// add movement in that direction
		FVector xAxis(1, 0, 0);
		AddMovementInput(xAxis, val);
	}
}

void ATDVCharacter::MoveRight(float val)
{
	FVector yAxis(0, 1, 0);
	AddMovementInput(yAxis, val);
}

void ATDVCharacter::InviteFriend()
{
	// Get the current game instance
	UMyGameInstance* Gi = Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	Gi->InviteFriend();
}

void ATDVCharacter::OnFire_Implementation()
{
	APlayerController* playercontroller = Cast<APlayerController>(GetController());

	if (IsValid(playercontroller))
	{
		FHitResult hitResult;
		playercontroller->GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, false, hitResult);

		hitResult.Location.Z = 0;

		FVector playerLocationPlane = GetActorLocation();
		playerLocationPlane.Z = 0;

		FRotator rotation = (hitResult.Location - playerLocationPlane).Rotation();
		FVector location = GetActorLocation() + FVector(0, 0, 200);

		FTransform transform(rotation, location);

		Server_Fire(transform);
	}
}

void ATDVCharacter::Server_Fire_Implementation(FTransform transform)
{
	UWorld* world = GetWorld();
	if (IsValid(world) && _projectileClass)
	{
		FActorSpawnParameters param;
		param.Owner = this;
		Instigator = this;


		AProjectile* projectile = world->SpawnActor<AProjectile>(_projectileClass, transform.GetLocation(), transform.GetRotation().Rotator(), param);
	}
}

bool ATDVCharacter::Server_Fire_Validate(FTransform transform)
{
	return true;
}

void ATDVCharacter::Server_LoseLife_Implementation(float amount)
{
	if (HasAuthority())
	{
		AMainGameState* MainGameState = GetWorld()->GetGameState<AMainGameState>();
		MainGameState->Server_LoseLife(amount);
	}
}

bool ATDVCharacter::Server_LoseLife_Validate(float amount)
{
	return true;
}

void ATDVCharacter::Server_GainLife_Implementation(float amount)
{
	if (HasAuthority())
	{
		AMainGameState* MainGameState = GetWorld()->GetGameState<AMainGameState>();
		MainGameState->Server_GainLife(amount);
	}
}

bool ATDVCharacter::Server_GainLife_Validate(float amount)
{
	return true;
}

bool ATDVCharacter::GetMousePositionOnAimingPlane(FVector& IntersectVector) const
{
	AMainGameController* Controller = Cast<AMainGameController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	ULocalPlayer* LocalPlayer = Cast<ULocalPlayer>(Controller->Player);
	bool bHit = false;
	if (LocalPlayer && LocalPlayer->ViewportClient)
	{
		FVector2D MousePosition;
		if (LocalPlayer->ViewportClient->GetMousePosition(MousePosition))
		{
			bHit = GetPlanePositionAtScreenPosition(MousePosition, IntersectVector);
		}
	}

	if (!bHit)	//If there was no hit we reset the results. This is redundant but helps Blueprint users
	{
		IntersectVector = FVector::ZeroVector;
	}

	return bHit;
}

bool ATDVCharacter::GetPlanePositionAtScreenPosition(
	const FVector2D ScreenPosition, 
	FVector& IntersectVector) const
{
	// Early out if we clicked on a HUD hitbox
	AMainGameController* Controller = Cast<AMainGameController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (Controller->GetHUD() != NULL && Controller->GetHUD()->GetHitBoxAtCoordinates(ScreenPosition, true))
	{
		return false;
	}

	FVector WorldOrigin;
	FVector WorldDirection;
	if (UGameplayStatics::DeprojectScreenToWorld(Controller, ScreenPosition, WorldOrigin, WorldDirection) == true)
	{
		IntersectVector = FMath::LinePlaneIntersection(WorldOrigin, WorldOrigin + WorldDirection * Controller->HitResultTraceDistance, GetActorLocation(), FVector::UpVector);
		return true;
	}

	return false;
}

void ATDVCharacter::AimUsingMouseCursor()
{
	AMainGameController* Controller = Cast<AMainGameController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	// Get the pawn location
	FVector PawnLocation = GetActorLocation();

	// Trace to whats beneath the mouse cursor
	FHitResult OutTraceResult;
	//GetHitResultUnderCursor(ECC_Pawn, false, OutTraceResult);
	FVector IntersectVector;
	GetMousePositionOnAimingPlane(IntersectVector);

	// Trace down through the aiming plane to see if we hit an actor that we can aim at
	static const FName NAME_MouseAimingTrace("MouseAimingTrace");
	FCollisionQueryParams CollisionQueryParams(NAME_MouseAimingTrace, true);
	bool bHit = GetWorld()->LineTraceSingleByChannel(OutTraceResult, IntersectVector, IntersectVector - FVector::UpVector * Controller->HitResultTraceDistance, ECC_Pawn, CollisionQueryParams);


	// If we hit something aim set that as our aim direction, otherwise aim at the point on the plane
	FVector Location = bHit ? OutTraceResult.ImpactPoint : IntersectVector;

	if (Location != FVector::ZeroVector)
	{
		DrawDebugLine(GetWorld(), PawnLocation, Location, FColor(255, 0, 0), false, -1, 0, 10.0f);
		if (bHit)
			DrawDebugLine(GetWorld(), IntersectVector, OutTraceResult.ImpactPoint, FColor(255, 255, 0), false, -1, 0, 10.0f);
	} 
	FRotator PlayerRot = UKismetMathLibrary::FindLookAtRotation(PawnLocation, Location);
	PlayerRot.Pitch = 0.f;
	SetActorRotation(PlayerRot);
}