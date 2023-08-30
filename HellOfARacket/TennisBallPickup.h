// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Pickup.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Kismet/GameplayStatics.h"
//#include "HellOfARacketCharacter.h"
#include "Components/SphereComponent.h"
#include "Components/TimelineComponent.h"
#include "TennisBallPickup.generated.h"

//class UCurveVector;

UCLASS()
class HELLOFARACKET_API ATennisBallPickup : public APickup
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ATennisBallPickup();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void ActivatePickup() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};