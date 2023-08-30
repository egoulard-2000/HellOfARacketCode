// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Enemy.h"
#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PickleBallSatan.generated.h"


UENUM()
enum Stage
{
	ServeStage UMETA(DisplayName = "ServeStage"),
	BusStage   UMETA(DisplayName = "BusStage"),
};

/**
 * 
 */
UCLASS()
class HELLOFARACKET_API APickleBallSatan : public AEnemy
{
	GENERATED_BODY()
	

public:
	// Sets default values for this character's properties
	APickleBallSatan();

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	UAnimSequence* MyAnimSequence;

	UFUNCTION(BlueprintImplementableEvent)
		void SwingAnimEvent();

	UPROPERTY(EditDefaultsOnly, Category = "Projectile")
	TSubclassOf<class AProjectile> projectileClass;
	
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TEnumAsByte<Stage> currStage = ServeStage;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float RallyCounter = 0.0f;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float MaxRallyCounter = 2.0f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float RallysWonByPlayer = 0.0f;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float RallysNeededToProgress = 2.0f;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float rallyLossCooldown = 1.0f;

	UFUNCTION(BlueprintCallable)
		void Reflect(AProjectile* proj);
	UFUNCTION(BlueprintCallable)
		void LoseRally();
	UFUNCTION(BlueprintCallable)
		void ResetReflect();

	FTimerHandle ResetReflectHandle;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void ShootAttack() override;

	void ProjDestroyedEventFunction(); 

	// Used for Serve Attack
	UPROPERTY(EditDefaultsOnly, Category = "Projectile")
	TSubclassOf<AActor> pickleball;

	// Used for Bus Attack
	UPROPERTY(EditDefaultsOnly, Category = "Projectile")
	TSubclassOf<AActor> launcher;

	// Used for Bus Attack
	UPROPERTY(EditDefaultsOnly, Category = "Projectile")
	TSubclassOf<class AProjectile> fireball;

	

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	USceneComponent* ProjectileSpawnPoint;

	// Moves the position of the projectile's location towards the player
	void RotateProjectileSpawnPointTowardsPlayer(FVector PlayerLocation);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	USceneComponent* FireballSpawnPoint;



private:
	UFUNCTION(BlueprintCallable)
		void Serve();

	UPROPERTY(EditAnywhere, Category = "satanSwingForce")
		float satanSwingForce; // force

	// AI variables
	class AAIController* MyController;
	class UBlackboardComponent* SatanBlackboard; 

	// Resets from the current state to serve state
	UFUNCTION(BlueprintCallable)
	void ResetToServeState();

	UFUNCTION(BlueprintCallable)
	void ResetFireAttack();

	UFUNCTION(BlueprintCallable)
	void UpdateTotalFireBreaths();

	float FireBreaths; 
	float MaxFireBreaths;
};
