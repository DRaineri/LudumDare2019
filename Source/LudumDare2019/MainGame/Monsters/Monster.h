// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Runtime/Engine/Classes/GameFramework/FloatingPawnMovement.h"
#include "Monster.generated.h"

UCLASS()
class LUDUMDARE2019_API AMonster : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AMonster();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditDefaultsOnly, Category = Behavior)
	class UBehaviorTree *MonsterBehavior;

	//UPROPERTY()
	//UFloatingPawnMovement* PawnMovement;
};
