// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ErrorDialogUserWidget.generated.h"

/**
 *
 */
UCLASS()
class LUDUMDARE2019_API UErrorDialogUserWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	UFUNCTION()
		void SetErrorMessage(FString NewErrorMessage);

	UPROPERTY(VisibleAnywhere, BlueprintReadonly, Category = "ErrorMessage")
		FString ErrorMessage;
};
