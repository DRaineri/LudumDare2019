// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "FMonsterData.h"
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

	virtual void GetLifetimeReplicatedProps(
		TArray<FLifetimeProperty>& OutLifetimeProps
	) const override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// The method used to take damages
	UFUNCTION(Server, Reliable, WithValidation)
	void Server_TakeDamages(float Damages);

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_KillMonster();

	UPROPERTY(EditDefaultsOnly, Category = Behavior)
	class UBehaviorTree *MonsterBehavior;

	UPROPERTY(Replicated, EditDefaultsOnly, Category = Config)
	FMonsterData MonsterData;

	UPROPERTY(Replicated, VisibleAnywhere, Category = Dying)
		bool bIsDying;
};
