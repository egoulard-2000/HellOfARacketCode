// Fill out your copyright notice in the Description page of Project Settings.

#include "FinalOrb.h"
#include "HellOfARacketCharacter.h"

AFinalOrb::AFinalOrb()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AFinalOrb::BeginPlay()
{
	Super::BeginPlay();
}

void AFinalOrb::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

/*
	Overiding Take Damage because the Final Orb isn't anything fancy
*/
float AFinalOrb::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	AProjectile* ProjectileActor = Cast<AProjectile>(DamageCauser);
	if (ProjectileActor == DamageCauser)
		return 0.0f;

	float DamageTaken = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	return DamageTaken;
}