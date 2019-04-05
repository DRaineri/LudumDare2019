// Fill out your copyright notice in the Description page of Project Settings.

#include "MyGameInstance.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Engine.h"

UMyGameInstance::UMyGameInstance(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	CurrentState = EGameState::Startup;
	EnableLan = false;
}

void UMyGameInstance::BeginShowMainMenu()
{
	if (!IsCurrentState(EGameState::Startup))
		return;

	ShowMainMenu();
}

void UMyGameInstance::ShowMainMenu()
{

	if (IsCurrentState(EGameState::Playing))
		UGameplayStatics::OpenLevel(this, FName("MainMenu"), true);

	if (!TransitionToState(EGameState::MainMenu))
		return;

	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (!MainMenu)
		MainMenu = CreateWidget<UUserWidget>(PlayerController, wMainMenu);

	FInputModeUIOnly InputMode;
	InputMode.SetWidgetToFocus(MainMenu->TakeWidget());
	PlayerController->SetInputMode(InputMode);
	PlayerController->bShowMouseCursor = true;
	PlayerController->bEnableClickEvents = true;

	MainMenu->AddToViewport();
}

void UMyGameInstance::ShowLoadingScreen()
{
	if (!TransitionToState(EGameState::LoadingScreen))
		return;

	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (!LoadingScreen)
		LoadingScreen = CreateWidget<UUserWidget>(PlayerController, wLoadingScreen);

	LoadingScreen->AddToViewport();

	FInputModeUIOnly InputMode;
	InputMode.SetWidgetToFocus(LoadingScreen->TakeWidget());
	PlayerController->SetInputMode(InputMode);
	PlayerController->bShowMouseCursor = true;
	PlayerController->bEnableClickEvents = true;
}

void UMyGameInstance::ShowServersList()
{
	if (!TransitionToState(EGameState::ServersList))
		return;

	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (!ServersList)
		ServersList = CreateWidget<UUserWidget>(PlayerController, wServersList);

	ServersList->AddToViewport();

	FInputModeUIOnly InputMode;
	InputMode.SetWidgetToFocus(ServersList->TakeWidget());
	PlayerController->SetInputMode(InputMode);
	PlayerController->bShowMouseCursor = true;
	PlayerController->bEnableClickEvents = true;
}

void UMyGameInstance::ShowErrorDialog(FString ErrorMessage)
{
	if (!TransitionToState(EGameState::ErrorDialog))
		return;

	DestroySession();

	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (!ErrorDialog)
		ErrorDialog = CreateWidget<UErrorDialogUserWidget>(PlayerController, wErrorDialog);

	ErrorDialog->SetErrorMessage(ErrorMessage);
	ErrorDialog->AddToViewport();

	FInputModeUIOnly InputMode;
	InputMode.SetWidgetToFocus(LoadingScreen->TakeWidget());
	PlayerController->SetInputMode(InputMode);
	PlayerController->bShowMouseCursor = true;
	PlayerController->bEnableClickEvents = true;
}

bool UMyGameInstance::IsCurrentState(EGameState InGameState)
{
	return CurrentState == InGameState;
}

bool UMyGameInstance::TransitionToState(EGameState DesiredState)
{
	if (IsCurrentState(DesiredState))
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Error : The desired state is already the current state %f"), (int)DesiredState));
		return false;
	}

	switch (CurrentState)
	{
	case EGameState::MainMenu:
		MainMenu->RemoveFromParent();
		break;
	case EGameState::ErrorDialog:
		ErrorDialog->RemoveFromParent();
		break;
	case EGameState::LoadingScreen:
		LoadingScreen->RemoveFromParent();
		break;
	case EGameState::ServersList:
		ServersList->RemoveFromParent();
		break;
	}

	CurrentState = DesiredState;
	return true;
}