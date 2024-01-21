// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LightSwitchActor.generated.h"

UCLASS()
class CROUCHMANCODER_API ALightSwitchActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALightSwitchActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//declare point light comp
	UPROPERTY(VisibleAnywhere, Category = "Light Switch")
	class UPointLightComponent* PointLight;

	//declare sphere comp
	UPROPERTY(VisibleAnywhere, Category = "Light Switch")
	class USphereComponent* LightSphere;

	//declare light intensity variable
	UPROPERTY(VisibleAnywhere, Category = "Light Switch")
	float LightIntensity;


	UFUNCTION(BlueprintCallable, Category = "Light Switch")
	void ToggleLight();
};
