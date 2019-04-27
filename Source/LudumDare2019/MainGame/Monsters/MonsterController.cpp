// Fill out your copyright notice in the Description page of Project Settings.

#include "MonsterController.h"
#include "Runtime/AIModule/Classes/BehaviorTree/BehaviorTree.h"
#include "Runtime/AIModule/Classes/Navigation/CrowdFollowingComponent.h"
#include "Runtime/AIModule/Classes/BehaviorTree/BlackboardComponent.h"


AMonsterController::AMonsterController(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<UCrowdFollowingComponent>(TEXT("PathFollowingComponent")))
{
	BlackboardComp = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackboardComp"));
	BehaviorComp = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviorComp"));
}

void AMonsterController::Possess(APawn* InPawn)
{
	Super::Possess(InPawn);

	AMonster* Monster = Cast<AMonster>(InPawn);
	MyPawn = Monster;

	if (Monster && Monster->MonsterBehavior)
	{
		BlackboardComp->InitializeBlackboard(*Monster->MonsterBehavior->BlackboardAsset);

		BehaviorComp->StartTree(*Monster->MonsterBehavior);
	}
}
