// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "LudumDare2019GameMode.h"
#include "LudumDare2019HUD.h"
#include "LudumDare2019Character.h"
#include "Ludumndare2019Controller.h"
#include "UObject/ConstructorHelpers.h"

ALudumDare2019GameMode::ALudumDare2019GameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPerson/Blueprints/FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;
	
	PlayerControllerClass = ALudumnDare2019Controller::StaticClass();

	// use our custom HUD class
	HUDClass = ALudumDare2019HUD::StaticClass();
}
