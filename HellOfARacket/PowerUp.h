// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Pickup.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PowerUp.generated.h"

UCLASS()
class HELLOFARACKET_API APowerUp : public APickup
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APowerUp();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// the type of powerup
	FName PowerUpName;

	void ActivatePickup() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Public way to access the PowerUp's name
	FName GetPowerUpName();

	// Blueprint callable FMOD event
	UFUNCTION(BlueprintImplementableEvent)
	void playFMODEvent();

private:

	UPROPERTY(VisibleAnywhere, Category = "Power")
		FName CurrentPowerUp;
};