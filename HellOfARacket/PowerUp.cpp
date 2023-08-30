// Fill out your copyright notice in the Description page of Project Settings.


#include "PowerUp.h"
#include "HellOfARacketCharacter.h"

// Sets default values
APowerUp::APowerUp()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void APowerUp::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void APowerUp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// return the PowerUp name
FName APowerUp::GetPowerUpName()
{
	return PowerUpName;
}

// Self-Explanatory
void APowerUp::ActivatePickup()
{
	// Add health to player here
	AHellOfARacketCharacter* PlayerCharacter = Cast<AHellOfARacketCharacter>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	if (PlayerCharacter == nullptr)
		return;

	CurrentPowerUp = GetPowerUpName();
	playFMODEvent();
	PlayerCharacter->SetCurrentPowerUp(CurrentPowerUp);
	

	Destroy();
}