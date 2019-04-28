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

void AMainGameState::Authority_StartGame()
{
	ensure(HasAuthority());
	if (CurrentGameState == EGameStateEnum::VE_WaitingForPlayers)
	{
		CurrentGameState = EGameStateEnum::VE_TransitionToArena;
		OnRep_CurrentGameStateUpdated();

		GetWorldTimerManager().SetTimer(_timerHandle, this, &AMainGameState::Authority_StartFight, 6.f, false);

	}
}


void AMainGameState::Authority_StartFight()
{
	ensure(HasAuthority());
	if (CurrentGameState == EGameStateEnum::VE_TransitionToArena)
	{
		CurrentGameState = EGameStateEnum::VE_FightInArena;
		OnRep_CurrentGameStateUpdated();
	}
}

void AMainGameState::Authority_EndFight()
{
	ensure(HasAuthority());
	if (CurrentGameState == EGameStateEnum::VE_FightInArena)
	{
		CurrentGameState = EGameStateEnum::VE_TransitionToShop;
		OnRep_CurrentGameStateUpdated();

		//GetWorldTimerManager().SetTimer(_timerHandle, this, &AMainGameState::Authority_StartFight, 6.f, false);
	}
}


void AMainGameState::OnRep_CurrentGameStateUpdated()
{
	switch (CurrentGameState)
	{
		case EGameStateEnum::VE_TransitionToArena:
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Game start in 6 seconds! Go to fighting area!"));
			OnGameStart.Broadcast();
			break;
		}
		case EGameStateEnum::VE_FightInArena:
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Fight for your life!"));
			OnFightStart.Broadcast();
			break;
		}
		case EGameStateEnum::VE_TransitionToShop:
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Round end, you have 6 seconds to go back to shop!"));
			OnFightEnd.Broadcast();
		}
	}
}

