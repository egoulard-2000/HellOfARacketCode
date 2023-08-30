// Fill out your copyright notice in the Description page of Project Settings.

#include "TennisBallPickup.h"
#include "HellOfARacketCharacter.h"

// Sets default values
ATennisBallPickup::ATennisBallPickup()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ATennisBallPickup::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ATennisBallPickup::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Self-Explanatory
void ATennisBallPickup::ActivatePickup()
{
	// Add health to player here
	AHellOfARacketCharacter* PlayerCharacter = Cast<AHellOfARacketCharacter>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	if (PlayerCharacter == nullptr)
		return;

	PlayerCharacter->SetBall();

	Destroy();
}