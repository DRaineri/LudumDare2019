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

	void Authority_SetActiveSpawner(bool isActive);

	void Authority_SpawnMonster();

	UPROPERTY(EditInstanceOnly)
	FName SpawnerId;
	
	UPROPERTY(EditDefaultsOnly)
	TSoftClassPtr<AMonster> MonsterClassToSpawn;

	UPROPERTY(EditDefaultsOnly)
	TSoftClassPtr<UParticleSystem> FxToTriggerOnSpawn;

	UPROPERTY(EditDefaultsOnly)
	float SpawnFrequency = 0.f;

protected :
	bool _isActiveSpawner = false;

	FTimerHandle _timerHandle;
};