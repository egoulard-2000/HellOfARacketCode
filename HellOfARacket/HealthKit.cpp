// Fill out your copyright notice in the Description page of Project Settings.

#include "HealthKit.h"
#include "HellOfARacketCharacter.h"


// Sets default values
AHealthKit::AHealthKit()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AHealthKit::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AHealthKit::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Self-Explanatory
void AHealthKit::ActivatePickup()
{
	// Add health to player here
	AHellOfARacketCharacter* PlayerCharacter = Cast<AHellOfARacketCharacter>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	if (PlayerCharacter == nullptr)
		return;

	PlayerCharacter->AddHealth(HealthAmount);

	Destroy();
}