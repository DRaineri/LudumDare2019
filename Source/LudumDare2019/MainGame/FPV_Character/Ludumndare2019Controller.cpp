
#include "Ludumndare2019Controller.h"
#include "Runtime/Engine/Classes/Engine/Engine.h"


void ALudumnDare2019Controller::SetupInputComponent()
{
	Super::SetupInputComponent();
	InputComponent->BindAction("SwitchPawn", IE_Released, this, &ALudumnDare2019Controller::SwitchPawn);
}

void ALudumnDare2019Controller::SwitchPawn()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Switch time !!"));
}
