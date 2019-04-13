#pragma once

#include "Engine/Classes/GameFramework/PlayerController.h"

#include "MainGameController.generated.h"

UCLASS()
class AMainGameController : public APlayerController
{
	GENERATED_BODY()

protected:
	virtual void SetupInputComponent() override;

	void SwitchPawn();
};