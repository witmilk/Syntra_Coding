// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "CrouchManCoderMySaveGame.generated.h"

/**
 * 
 */
UCLASS()
class CROUCHMANCODER_API UCrouchManCoderMySaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FTransform PlayerTransform;
};
