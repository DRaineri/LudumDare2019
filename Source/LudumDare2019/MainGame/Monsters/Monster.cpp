// Fill out your copyright notice in the Description page of Project Settings.

#include "Monster.h"
#include "MonsterController.h"
#include "Net/UnrealNetwork.h"
#include "Engine.h"

// Sets default values
AMonster::AMonster()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	AIControllerClass = AMonsterController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

// Called when the game starts or when spawned
void AMonster::BeginPlay()
{
	Super::BeginPlay();
	
	MonsterData.CurrentHealth = MonsterData.MaxHealth;
}

void AMonster::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AMonster, MonsterData);
}

// Called every frame
void AMonster::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMonster::Server_TakeDamages_Implementation(float Damages)
{
	if (!HasAuthority())
		return;

	MonsterData.CurrentHealth -= Damages;
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Monster life: %f"), MonsterData.CurrentHealth));
	if (MonsterData.CurrentHealth <= 0.f)
	{
		Destroy();
	}
}

bool AMonster::Server_TakeDamages_Validate(float Damages)
{
	return true;
}