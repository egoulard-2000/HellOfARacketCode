// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Enemy.h"
#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GolfballDemon.generated.h"


UCLASS()
class HELLOFARACKET_API AGolfballDemon : public AEnemy
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AGolfballDemon();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void ShootAttack() override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditDefaultsOnly, Category="Projectile")
	TSubclassOf<class AProjectile> projectileClass;

private:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta = (AllowPrivateAccess = "true"))
	USceneComponent *ProjectileSpawnPoint;

	// Moves the position of the projectile's location towards the player
	void RotateProjectileSpawnPointTowardsPlayer(FVector PlayerLocation);
};
