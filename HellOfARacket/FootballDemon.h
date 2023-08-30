// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemy.h"
#include "GameFramework/Character.h"
#include "CamShake_FootballDemonGP.h"
#include "FootballDemon.generated.h"


UCLASS()
class HELLOFARACKET_API AFootballDemon : public AEnemy
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AFootballDemon();
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool InGroundPoundState = false;
	void ChargeAttack();
	void GroundSmashAttack();
	void EnableAttackCollision(const bool Enabled);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Landed on Ground Event
	virtual void Landed(const FHitResult& Hit) override;

	UPROPERTY(EditAnywhere, Category = "Charge Collider")
		class UBoxComponent* ChargeCollider;

private:

#pragma region Charge Attack Parameters

	UPROPERTY(EditAnywhere, Category = "Football Demon|Charge Attack", meta = (UIMin = 5000.0, UIMax = 10000.0))
		float DashDistance = 5500.0f;

	UPROPERTY(EditAnywhere, Category = "Football Demon|Charge Attack", meta = (UIMin = 0.0, UIMax = 100.0))
		float ChargeAttack_DamageAmount = 40.0f;

	UPROPERTY(EditAnywhere, Category = "Football Demon|Charge Attack", meta = (UIMin = 1000.0, UIMax = 5000.0))
		float ChargeAttack_KnockbackDistance = 1500.0f;

#pragma endregion Charge Attack Parameters

#pragma region Ground Pound Attack Parameters

	UPROPERTY(EditAnywhere, Category = "Football Demon|Ground Pound Attack", meta = (UIMin = 0.0, UIMax = 100.0))
		float GroundPoundAttack_DamageAmount = 50.0f;

	UPROPERTY(EditAnywhere, Category = "Football Demon|Ground Pound Attack", meta = (UIMin = 2000.0, UIMax = 5000.0))
		float GroundPoundAttack_KnockbackMaxDistance = 2000.0f;
	
	UPROPERTY(EditAnywhere, Category = "Football Demon|Ground Pound Attack", meta = (UIMin = 100.0, UIMax = 1000.0))
		float GroundPoundImpactRadius = 600.0f;

	UPROPERTY(EditAnywhere, Category = "Football Demon|Ground Pound Attack", meta = (UIMin = 3.0, UIMax = 5.0))
		float GroundPoundSpeed = 3.2f;

	UPROPERTY(EditAnywhere, Category = "Football Demon|Ground Pound Attack")
		float AirTime = 1.0f;

	UPROPERTY(EditAnywhere, Category = "Football Demon|Ground Pound Attack")
		float JumpHeight = 2000.0f;

	UPROPERTY(EditAnywhere, Category = "Football Demon|Camera|Shake")
		TSubclassOf<UMatineeCameraShake> FootballDemonGroundPoundShake;

#pragma endregion Ground Pound Attack Parameters

#pragma region Helper Properties

	TArray<FHitResult> HitActors;
	FTimerHandle AirTimeHandle;
	void EndAirTime();

#pragma endregion Helper Properties

};
