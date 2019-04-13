// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "MainGameMode.h"
#include "FPV_Character/FPVHUD.h"
#include "FPV_Character/FPVCharacter.h"
#include "MainGameController.h"
#include "UObject/ConstructorHelpers.h"

AMainGameMode::AMainGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPerson/Blueprints/FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;
	
	PlayerControllerClass = AMainGameController::StaticClass();

	// use our custom HUD class
	HUDClass = AFPVHUD::StaticClass();
}
