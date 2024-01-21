// Fill out your copyright notice in the Description page of Project Settings.


#include "LightSwitchActor.h"
#include "Components/PointLightComponent.h"
#include "Components/SphereComponent.h"
// Sets default values
ALightSwitchActor::ALightSwitchActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	LightIntensity = 3000.0f;

	PointLight = CreateDefaultSubobject<UPointLightComponent>(TEXT("Point Light"));
	PointLight->Intensity = LightIntensity;
	PointLight->SetVisibility(true);
	RootComponent = PointLight;

	LightSphere = CreateDefaultSubobject<USphereComponent>(TEXT("Light Sphere Component"));
	LightSphere->InitSphereRadius(300.0f);
	LightSphere->SetCollisionProfileName(TEXT("Trigger"));
	LightSphere->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
	LightSphere->SetupAttachment(RootComponent);

}

// Called when the game starts or when spawned
void ALightSwitchActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ALightSwitchActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ALightSwitchActor::ToggleLight()
{
	PointLight->ToggleVisibility();
}