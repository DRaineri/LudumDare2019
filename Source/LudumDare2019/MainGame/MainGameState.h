// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "Engine.h"
#include "MainGameState.generated.h"

UENUM(BlueprintType)		//"BlueprintType" is essential to include
enum class EGameStateEnum : uint8
{
	VE_WaitingForPlayers 	UMETA(DisplayName = "Waiting for player"),
	VE_TransitionToArena 	UMETA(DisplayName = "Transition to arena"),
	VE_FightInArena	UMETA(DisplayName = "Fight in arena"),
	VE_TransitionToShop	UMETA(DisplayName = "Transition to shop"),
	VE_ShoppingTime	UMETA(DisplayName = "ShoppingTime")
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnGameStart);

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

	UFUNCTION()
	void Authority_StartGame();

	UFUNCTION()
	void OnRep_CurrentGameStateUpdated();

	UPROPERTY(BlueprintReadWrite, BlueprintAssignable)
	FOnGameStart OnGameStart;

protected:

	UPROPERTY(Replicated, BlueprintReadOnly)
	float PlayersLife;

	UPROPERTY(Replicated, BlueprintReadOnly)
	float PlayersMaxLife;

	UPROPERTY(ReplicatedUsing = OnRep_CurrentGameStateUpdated)
	EGameStateEnum CurrentGameState = EGameStateEnum::VE_WaitingForPlayers;

	UPROPERTY(Replicated)
	int32 CurrentLevel = -1;

	virtual void GetLifetimeReplicatedProps(
		TArray<FLifetimeProperty>& OutLifetimeProps
	) const override;
};
