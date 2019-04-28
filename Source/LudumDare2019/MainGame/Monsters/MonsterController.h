// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Monster.h"
#include "MonsterController.generated.h"

/**
 * 
 */
UCLASS()
class LUDUMDARE2019_API AMonsterController : public AAIController
{
	GENERATED_BODY()

public:
	AMonsterController(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(transient)
	class UBlackboardComponent* BlackboardComp;

	UPROPERTY(transient)
	class UBehaviorTreeComponent* BehaviorComp;

	virtual void Possess(class APawn* InPawn) override;


	UFUNCTION(BlueprintCallable)
	void SetTargetFocus();

protected:
	UPROPERTY()
	AMonster* MyPawn;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	APawn* MyTarget;

	APawn* GetClosestPlayer(TArray<APawn*> Players);
};
