// Copyright Epic Games, Inc. All Rights Reserved.

#include "CrouchManCoderGameMode.h"
#include "CrouchManCoderCharacter.h"
#include "UObject/ConstructorHelpers.h"

ACrouchManCoderGameMode::ACrouchManCoderGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
