
#include "MainGameController.h"
#include "Runtime/Engine/Classes/Engine/Engine.h"


void AMainGameController::SetupInputComponent()
{
	Super::SetupInputComponent();
	InputComponent->BindAction("SwitchPawn", IE_Released, this, &AMainGameController::SwitchPawn);
}

void AMainGameController::SwitchPawn()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Switch time !!"));
}
