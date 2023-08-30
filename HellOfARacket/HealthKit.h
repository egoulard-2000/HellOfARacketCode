// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Pickup.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SphereComponent.h"
#include "Components/TimelineComponent.h"
#include "HealthKit.generated.h"

//class UCurveVector;

UCLASS()
class HELLOFARACKET_API AHealthKit : public APickup
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AHealthKit();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void ActivatePickup() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(EditAnywhere)
		float HealthAmount = 10.0f;
};
