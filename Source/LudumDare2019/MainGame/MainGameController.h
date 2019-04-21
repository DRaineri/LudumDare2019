#pragma once

#include "Engine/Classes/GameFramework/PlayerController.h"

#include "MainGameController.generated.h"

UCLASS()
class AMainGameController : public APlayerController
{
	GENERATED_BODY()

	AMainGameController();

protected:
	virtual void SetupInputComponent() override;

	UFUNCTION(Server, reliable, withValidation)
	void Server_SwitchPawn();

	TSubclassOf<APawn> _firstPersonPawnClass;
	TSubclassOf<APawn> _topDownPawnClass;
};