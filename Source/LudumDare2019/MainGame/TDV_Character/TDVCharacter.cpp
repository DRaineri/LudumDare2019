
#include "TDVCharacter.h"

#include "UObject/ConstructorHelpers.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/SpringArmComponent.h"
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

void ATDVCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Bind movement events
	PlayerInputComponent->BindAxis("MoveForward", this, &ATDVCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ATDVCharacter::MoveRight);
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

void ATDVCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}