// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "FPVCharacter.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/InputSettings.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "MotionControllerComponent.h"
#include "XRMotionControllerBase.h" // for FXRMotionControllerBase::RightHandSourceId
#include "Runtime/Engine/Classes/Engine/Engine.h"
#include "MyGameInstance.h"
#include "MainGame/MainGameState.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"

DEFINE_LOG_CATEGORY_STATIC(LogFPChar, Warning, All);

//////////////////////////////////////////////////////////////////////////
// AFPVCharacter

AFPVCharacter::AFPVCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = false;

	// Create a first person camera component.
	FPSCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	// Attach the camera component to our capsule component.
	FPSCameraComponent->SetupAttachment(
		GetMesh(),
		TEXT("Head")
	);
	FPSCameraComponent->bUsePawnControlRotation = true;

	GetMesh()->VisibilityBasedAnimTickOption = EVisibilityBasedAnimTickOption::AlwaysTickPoseAndRefreshBones;
}


void AFPVCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	GetMesh()->SetHiddenInGame(false, true);
	FPSCameraComponent->SetHiddenInGame(true, true);

	if (IsLocallyControlled())
	{
		if (!FirstPersonWidget)
			FirstPersonWidget = CreateWidget<UUserWidget>(GetWorld(), wFirstPersonWidget);
		FirstPersonWidget->AddToViewport();
	}
}

void AFPVCharacter::Destroyed()
{
	Super::Destroyed();

	if (IsLocallyControlled())
	{
		if (FirstPersonWidget)
			FirstPersonWidget->RemoveFromParent();
	}
}

//////////////////////////////////////////////////////////////////////////
// Input

void AFPVCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// set up gameplay key bindings
	check(PlayerInputComponent);
	   
	// Bind jump events
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	// Bind fire event
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &AFPVCharacter::OnFire);

	// Bind movement events
	PlayerInputComponent->BindAxis("MoveForward", this, &AFPVCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AFPVCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);


	PlayerInputComponent->BindAction("InviteFriend", EInputEvent::IE_Pressed, this, &AFPVCharacter::InviteFriend);
}

void AFPVCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	APlayerController* playerController = Cast<APlayerController>(NewController);
	if (IsValid(playerController))
	{
		playerController->bShowMouseCursor = false;
	}
}


void AFPVCharacter::MoveForward(float Value)
{
	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorForwardVector(), Value);
	}
}

void AFPVCharacter::MoveRight(float Value)
{
	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorRightVector(), Value);
	}
}

void AFPVCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AFPVCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void AFPVCharacter::InviteFriend()
{
	// Get the current game instance
	UMyGameInstance* Gi = Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	Gi->InviteFriend();
}

void AFPVCharacter::OnFire_Implementation()
{
	Server_Fire();
}

void AFPVCharacter::Server_Fire_Implementation()
{
	UWorld* world = GetWorld();
	if (IsValid(world) && _projectileClass)
	{
		FActorSpawnParameters param;
		param.Owner = this;
		Instigator = this;

		FVector spawnLocation = GetMesh()->GetSocketLocation("FirePlaceSocket");
		FRotator spawnRotation = GetControlRotation();

		AProjectile* projectile = world->SpawnActor<AProjectile>(_projectileClass, spawnLocation, spawnRotation, param);
	}
}

bool AFPVCharacter::Server_Fire_Validate()
{
	return true;
}

void AFPVCharacter::Server_LoseLife_Implementation(float amount)
{
	if (HasAuthority())
	{
		AMainGameState* MainGameState = GetWorld()->GetGameState<AMainGameState>();
		MainGameState->Server_LoseLife(amount);
	}
}

bool AFPVCharacter::Server_LoseLife_Validate(float amount)
{
	return true;
}

void AFPVCharacter::Server_GainLife_Implementation(float amount)
{
	if (HasAuthority())
	{
		AMainGameState* MainGameState = GetWorld()->GetGameState<AMainGameState>();
		MainGameState->Server_GainLife(amount);
	}
}

bool AFPVCharacter::Server_GainLife_Validate(float amount)
{
	return true;
}