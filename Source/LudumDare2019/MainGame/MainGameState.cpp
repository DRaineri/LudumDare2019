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
	DOREPLIFETIME(AMainGameState, CurrentGameState);
	DOREPLIFETIME(AMainGameState, CurrentLevel);

}

void AMainGameState::Server_LoseLife_Implementation(float amount)
{
	PlayersLife -= amount;

	if (PlayersLife <= 0.f)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("You are Dead!"));
		PlayersLife = PlayersMaxLife;
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

void AMainGameState::Auhtority_StartGame()
{
	ensure(HasAuthority());
	if (CurrentGameState == EGameStateEnum::VE_WaitingForPlayers)
	{
		CurrentGameState = EGameStateEnum::VE_TransitionToArena;
	}
}


void AMainGameState::OnRep_CurrentGameStateUpdated()
{
	switch (CurrentGameState)
	{
	case EGameStateEnum::VE_TransitionToArena:
	{

	}

	}
}

