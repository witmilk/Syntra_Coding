// Fill out your copyright notice in the Description page of Project Settings.


#include "ChestCannonComponent.h"

// Sets default values for this component's properties
UChestCannonComponent::UChestCannonComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UChestCannonComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UChestCannonComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UChestCannonComponent::Fire(FTransform origin, FVector direction)
{
	FVector spawnLocation = origin.GetLocation() + (direction * 100);
	AActor* cannonBallActor = GetWorld()->SpawnActor<AActor>(Projectile, spawnLocation, FRotator::ZeroRotator);
	if (cannonBallActor != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Fire in %s"), *GetNameSafe(this));

	}
}