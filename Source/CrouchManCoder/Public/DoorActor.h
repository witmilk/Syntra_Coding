// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Components/TimelineComponent.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DoorActor.generated.h"


UCLASS()
class CROUCHMANCODER_API ADoorActor : public AActor
{
    GENERATED_BODY()

public:
    // Sets default values for this actor's properties
    ADoorActor();

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

public:
    // Called every frame
    virtual void Tick(float DeltaTime) override;


protected:

    //MeshComponents to represent Door assets
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    UStaticMeshComponent* DoorFrame;
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    UStaticMeshComponent* Door;

    //TimelineComponent to animate Door meshes
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    UTimelineComponent* DoorTimelineComp;

    //BoxComponent which will be used as our Proximity volume.
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    class UBoxComponent* DoorProxVolume;

public:
    // Variable to hold the Curve asset
    UPROPERTY(EditAnywhere)
    UCurveFloat* DoorTimelineFloatCurve;

private:
    //Float Track Signature to handle our update track event
    FOnTimelineFloat UpdateFunctionFloat;

    //Function which updates our Door's relative location with the timeline graph
    UFUNCTION()
    void UpdateTimelineComp(float Output);

    // Begin and End Overlap Events for our DoorProxVolume
    UFUNCTION()
    void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
    UFUNCTION()
    void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

};