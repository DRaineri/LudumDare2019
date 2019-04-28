#pragma once

#include "CoreMinimal.h"
#include "FMonsterData.generated.h"

USTRUCT(BlueprintType)
struct FMonsterData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadonly, Category = Health)
	float CurrentHealth;

	UPROPERTY(EditDefaultsOnly, BlueprintReadonly, Category = Health)
	float MaxHealth;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Damages)
		float Damages;
};