
#include "MainGameController.h"
#include "Runtime/Engine/Classes/Engine/Engine.h"

#include "FPV_Character/FPVCharacter.h"
#include "TDV_Character/TDVCharacter.h"
#include "UObject/ConstructorHelpers.h"

AMainGameController::AMainGameController()
{
	ConstructorHelpers::FClassFinder<APawn> FirstPersonPawnClassFinder(TEXT("/Game/FirstPerson/Blueprints/FirstPersonCharacter"));
	_firstPersonPawnClass = FirstPersonPawnClassFinder.Class;

	ConstructorHelpers::FClassFinder<APawn> TopDownPawnClassFinder(TEXT("/Game/TopDown/Blueprints/TopDownCharacter"));
	_topDownPawnClass = TopDownPawnClassFinder.Class;
}

void AMainGameController::SetupInputComponent()
{
	Super::SetupInputComponent();
	InputComponent->BindAction("SwitchPawn", IE_Released, this, &AMainGameController::SwitchPawn);
}

void AMainGameController::SwitchPawn()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Switch time !!"));
	
	UWorld* world = GetWorld();
	APawn* previousPawn = GetPawn();

	if (IsValid(world) && IsValid(previousPawn))
	{
		FTransform pawnTransform = previousPawn->GetTransform();

		APawn* newPawn = nullptr;
		if (previousPawn->IsA(_topDownPawnClass))
		{
			newPawn = world->SpawnActor<APawn>(_firstPersonPawnClass, pawnTransform);
		}
		else if(previousPawn->IsA(_firstPersonPawnClass))
		{
			newPawn = world->SpawnActor<APawn>(_topDownPawnClass, pawnTransform);
		}
		else 
		{
			ensureMsgf(false, TEXT("Controller doesn't have a right pawn possessed (top down or first person)"));
			return;
		}

		previousPawn->Destroy();
		Possess(newPawn);
	}	
}
