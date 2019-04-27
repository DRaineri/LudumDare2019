
#include "TDVCharacter.h"

#include "UObject/ConstructorHelpers.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/SpringArmComponent.h"
#include "MyGameInstance.h"
#include "MainGame/MainGameState.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
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