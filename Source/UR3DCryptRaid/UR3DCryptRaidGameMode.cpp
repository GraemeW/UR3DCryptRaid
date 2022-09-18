// Copyright Epic Games, Inc. All Rights Reserved.

#include "UR3DCryptRaidGameMode.h"
#include "UR3DCryptRaidCharacter.h"
#include "UObject/ConstructorHelpers.h"

AUR3DCryptRaidGameMode::AUR3DCryptRaidGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

}
