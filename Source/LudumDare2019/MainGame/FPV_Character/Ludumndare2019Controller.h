#pragma once

#include "Engine/Classes/GameFramework/PlayerController.h"

#include "Ludumndare2019Controller.generated.h"

UCLASS()
class ALudumnDare2019Controller : public APlayerController
{
	GENERATED_BODY()

protected:
	virtual void SetupInputComponent() override;

	void SwitchPawn();
};