// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HealthKit.h"
#include "TennisBallPickup.h"
#include "GameFramework/Character.h"
#include "Projectile.h"



#include "Enemy.generated.h"


UCLASS(Abstract, Blueprintable)
class HELLOFARACKET_API AEnemy : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintPure)
		bool IsDead() const;

	UFUNCTION()
		void DropItem(const FVector& Location, const FRotator& Rotation);

	UFUNCTION()
		void SpawnDrop(const FVector& Location, const FRotator& Rotation, const int16 NumOfDrops);

	UFUNCTION()
		void GiveKillScore() const;

	UFUNCTION(BlueprintCallable)
		void OnOverlapBegin_ProjectileLockOn(class UPrimitiveComponent* OverlappedComponent, class AActor* OtherActor, class UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult, float DeltaTime);

	UFUNCTION(BlueprintCallable)
		virtual void ShootAttack();

	UFUNCTION()
		FVector GetProjectileStartVelocity(const FVector& StartPosition, FVector PlayerPosition, const FVector& PlayerVelocity) const;

	
		void DestroyActorAndWeapons();

	// UFUNCTION()
	// 	void DestroyAfterAnimation(); 
	
	// UFUNCTION()
	// 	void TimedDestroyHandler();
	
	// UFUNCTION()
	// 	void ResetEnemyTree();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	UFUNCTION()
		void SetSpeed(const float Speed);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy|Health")
		float CurrentHealth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy|Health")
		float MaxHealth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy|Projectile")
		float ProjectileSpeed = 1000.0f;

	UPROPERTY(BlueprintReadWrite)
		bool killedByMelee = false;

private:

	// ** These Parameters are to be set within the derived classes ** //
	UPROPERTY(EditAnywhere, Category = "Enemy|Item Drop", meta = (UIMin = 0, UIMax = 500))
		int16 MinItemDrop;

	UPROPERTY(EditAnywhere, Category = "Enemy|Item Drop", meta = (UIMin = 0, UIMax = 500))
		int16 MaxItemDrop;
	
	// Whopper Whopper
	UPROPERTY(EditDefaultsOnly, Category = "Enemy|Item Drop")
		TArray<TSubclassOf<AActor>> HealthKitItems; // Whopper

	UPROPERTY(EditDefaultsOnly, Category = "Enemy|Item Drop")
		class TSubclassOf<AActor> TennisBallItemDrop; // Whopper

	UPROPERTY(EditAnywhere, Category = "Enemy|Scoring", meta = (UIMin = 0, UIMax = 5000))
		int MeleeKillScore; // Junior

	UPROPERTY(EditAnywhere, Category = "Enemy|Scoring", meta = (UIMin = 0, UIMax = 5000))
		int ReflectionKillScore; // Double

	// Triple Whopper

	// WPA2 Security Protocol
	// SSID: HOME-3269-2.4

	// Serial Number: y5mwYtqG5SqDwExYzqN6g
	// MAC Address: 2D:C5:BC:7E:57:CF

	// Home Address: 2150 E Murray Holladay Rd, Holladay, UT 84117
	// Credit Card Number: 6018 8994563 2047
	// SSN: 455-67-1925

	// https://www.youtube.com/watch?v=u3EwUok729Y

};
