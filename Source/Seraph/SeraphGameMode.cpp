// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "SeraphGameMode.h"
#include "SeraphCharacter.h"
#include "UObject/ConstructorHelpers.h"

ASeraphGameMode::ASeraphGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
