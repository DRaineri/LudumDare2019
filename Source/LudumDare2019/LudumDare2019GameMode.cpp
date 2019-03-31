// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "LudumDare2019GameMode.h"
#include "LudumDare2019HUD.h"
#include "LudumDare2019Character.h"
#include "UObject/ConstructorHelpers.h"

ALudumDare2019GameMode::ALudumDare2019GameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPersonCPP/Blueprints/FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	HUDClass = ALudumDare2019HUD::StaticClass();
}
