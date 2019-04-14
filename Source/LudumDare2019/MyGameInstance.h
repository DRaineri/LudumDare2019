// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Runtime/UMG/Public/Blueprint/UserWidget.h"
#include "Menus/ErrorDialogUserWidget.h"
#include "MyGameInstance.generated.h"

UENUM(BlueprintType)
enum class EGameState : uint8
{
	Startup,
	MainMenu,
	ServersList,
	LoadingScreen,
	ErrorDialog,
	Playing
};

/**
 *
 */
UCLASS()
class LUDUMDARE2019_API UMyGameInstance : public UGameInstance
{
	GENERATED_BODY()

		UMyGameInstance(const FObjectInitializer& ObjectInitializer);

public:
	UFUNCTION(BlueprintCallable)
		void BeginShowMainMenu();

	UFUNCTION(BlueprintCallable)
		void ShowMainMenu();

	UFUNCTION(BlueprintCallable)
		void ShowLoadingScreen();

	UFUNCTION(BlueprintCallable)
		void ShowServersList();

	UFUNCTION(BlueprintCallable)
		void ShowErrorDialog(FString ErrorMessage);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
		void HostGame();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
		void DestroySession();

	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
		TSubclassOf<class UUserWidget> wMainMenu;
	UUserWidget* MainMenu;

	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
		TSubclassOf<class UUserWidget> wLoadingScreen;
	UUserWidget* LoadingScreen;

	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
		TSubclassOf<class UUserWidget> wServersList;
	UUserWidget* ServersList;

	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
		TSubclassOf<class UErrorDialogUserWidget> wErrorDialog;
	UErrorDialogUserWidget* ErrorDialog;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Config")
		bool EnableLan;

protected:
	bool IsCurrentState(EGameState InGameState);
	bool TransitionToState(EGameState DesiredState);

	EGameState CurrentState;
};
