// Fill out your copyright notice in the Description page of Project Settings.

#include "MonsterController.h"
#include "Runtime/AIModule/Classes/BehaviorTree/BehaviorTree.h"
#include "Runtime/AIModule/Classes/Navigation/CrowdFollowingComponent.h"
#include "Runtime/AIModule/Classes/BehaviorTree/BlackboardComponent.h"
#include "MainGame/FPV_Character/FPVCharacter.h"
#include "MainGame/TDV_Character/TDVCharacter.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include <limits>


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


void AMonsterController::SetTargetFocus()
{
	APawn* Target = NULL;
	TArray<APawn*> Players;
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AFPVCharacter::StaticClass(), FoundActors);

	for (AActor* Actor : FoundActors)
	{
		Players.Add(Cast<APawn>(Actor));
	}
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATDVCharacter::StaticClass(), FoundActors);
	for (AActor* Actor : FoundActors)
	{
		Players.Add(Cast<APawn>(Actor));
	}

	APawn* ClosestPlayer = GetClosestPlayer(Players);
	Target = ClosestPlayer;

	AActor* Actor = Cast<AActor>(BlackboardComp->GetValueAsObject("TargetReference"));
	if (Target != Actor)
	{
		BlackboardComp->SetValueAsObject("NextTargetReference", Target);
		MyTarget = Target;
	}
}

APawn* AMonsterController::GetClosestPlayer(TArray<APawn*> Players)
{
	float ClosestDistance = std::numeric_limits<float>::max();
	APawn* ClosestPlayer = NULL;
	for (int i = 0; i < Players.Num(); i++) {
		APawn* Player = Players[i];
		float DistanceTo = MyPawn->GetDistanceTo(Player);
		if (DistanceTo < ClosestDistance)
		{
			ClosestDistance = DistanceTo;
			ClosestPlayer = Player;
		}
	}

	return ClosestPlayer;
}