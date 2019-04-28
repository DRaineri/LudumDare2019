
#include "MainGameController.h"
#include "Runtime/Engine/Classes/Engine/Engine.h"

#include "FPV_Character/FPVCharacter.h"
#include "TDV_Character/TDVCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "LudumDare2019/MainGame/MainGameState.h"

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
	InputComponent->BindAction("SwitchPawn", IE_Released, this, &AMainGameController::Server_SwitchPawn);
	InputComponent->BindAction("StartGame", IE_Released, this, &AMainGameController::Server_StartGame);
}

void AMainGameController::Server_SwitchPawn_Implementation()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Switch time !!"));
	
	UWorld* world = GetWorld();
	APawn* previousPawn = GetPawn();

	if (IsValid(world) && IsValid(previousPawn))
	{
		FTransform pawnTransform = previousPawn->GetActorTransform();

		APawn* newPawn = nullptr;
		FActorSpawnParameters parameters;
		parameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		if (previousPawn->IsA(_topDownPawnClass))
		{
			newPawn = world->SpawnActor<APawn>(_firstPersonPawnClass, pawnTransform, parameters);
		}
		else if(previousPawn->IsA(_firstPersonPawnClass))
		{
			newPawn = world->SpawnActor<APawn>(_topDownPawnClass, pawnTransform, parameters);
		}
		else 
		{
			ensureMsgf(false, TEXT("Controller doesn't have a right pawn possessed (top down or first person)"));
			return;
		}

		ensureMsgf(IsValid(newPawn), TEXT("Invalid new pawn created"));

		previousPawn->Destroy();
		Possess(newPawn);
	}	
}

bool AMainGameController::Server_SwitchPawn_Validate()
{
	return true;
}

void AMainGameController::Server_StartGame_Implementation()
{
	UWorld* world = GetWorld();
	if (IsValid(world))
	{
		AMainGameState* gameState = world->GetGameState<AMainGameState>();
		if (IsValid(gameState))
		{

		}
	}
}

bool AMainGameController::Server_StartGame_Validate()
{
	return true;
}
