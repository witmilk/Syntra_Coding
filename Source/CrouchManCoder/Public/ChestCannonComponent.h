// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ChestCannonComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CROUCHMANCODER_API UChestCannonComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UChestCannonComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;


public:

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AActor> Projectile;

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	virtual void Fire(FTransform origin, FVector direction);
		
};
