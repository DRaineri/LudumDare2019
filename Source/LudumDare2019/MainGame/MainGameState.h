// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "Engine.h"
#include "MainGameState.generated.h"

/**
 * 
 */
UCLASS()
class LUDUMDARE2019_API AMainGameState : public AGameStateBase
{
	GENERATED_BODY()

	AMainGameState();

public:
	UFUNCTION(Server, Reliable, WithValidation, BlueprintCallable)
	void Server_LoseLife(float amount);

	UFUNCTION(Server, Reliable, WithValidation, BlueprintCallable)
	void Server_GainLife(float amount);

protected:

	UPROPERTY(Replicated, BlueprintReadOnly)
	float PlayersLife;

	UPROPERTY(Replicated, BlueprintReadOnly)
	float PlayersMaxLife;

	virtual void GetLifetimeReplicatedProps(
		TArray<FLifetimeProperty>& OutLifetimeProps
	) const override;
};
