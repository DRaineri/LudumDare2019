// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LudumDare2019/MainGame/Monsters/Monster.h"

#include "Spawner.generated.h"


UCLASS(BlueprintType)
class ASpawner : public AActor
{
	GENERATED_BODY()

public :
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	void Authority_SetActiveSpawner(bool isActive);

	void Authority_SpawnMonster();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SetSpawnerVisibility(bool newVisibility);

	UPROPERTY(EditInstanceOnly)
	FName SpawnerId;
	
	UPROPERTY(EditDefaultsOnly)
	TSoftClassPtr<AMonster> MonsterClassToSpawn;

	UPROPERTY(EditDefaultsOnly)
	TSoftClassPtr<UParticleSystem> FxToTriggerOnSpawn;

	UPROPERTY(EditDefaultsOnly)
	float SpawnFrequency = 10.f;

	UPROPERTY(EditDefaultsOnly)
	int32 NumberOfMonsterToSpawn = 1;

protected :
	bool _isActiveSpawner = false;

	int32 _nbMonsterSpawnedSinceLastActivation;

	FTimerHandle _timerHandle;
};