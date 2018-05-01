// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "S05TestingGroundsGameMode.h"
#include "S05TestingGroundsHUD.h"
#include "Player/FirstPersonCharacter.h"
#include "UObject/ConstructorHelpers.h"

AS05TestingGroundsGameMode::AS05TestingGroundsGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/Player/Behavior/FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	HUDClass = AS05TestingGroundsHUD::StaticClass();
}
