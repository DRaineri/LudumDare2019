// Fill out your copyright notice in the Description page of Project Settings.

#include "MainGameState.h"
#include "UnrealNetwork.h"

AMainGameState::AMainGameState()
	: Super()
{
	PlayersMaxLife = 100.f;
	PlayersLife = PlayersMaxLife;
}

void AMainGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AMainGameState, PlayersMaxLife);
	DOREPLIFETIME(AMainGameState, PlayersLife);
}

void AMainGameState::Server_LoseLife_Implementation(float amount)
{
	PlayersLife -= amount;

	if (PlayersLife <= 0.f)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("You are Dead!"));
		PlayersLife = 100.f;
	}
}

bool AMainGameState::Server_LoseLife_Validate(float amount)
{
	return true;
}

void AMainGameState::Server_GainLife_Implementation(float amount)
{
	PlayersLife += amount;

	if (PlayersLife >= PlayersMaxLife)
	{
		PlayersLife = PlayersMaxLife;
	}
}

bool AMainGameState::Server_GainLife_Validate(float amount)
{
	return true;
}