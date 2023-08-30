// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemy.h"
#include "GameFramework/Character.h"
#include "Components/BoxComponent.h"
#include "SoccerDemon.generated.h"

UCLASS()
class HELLOFARACKET_API ASoccerDemon : public AEnemy
{
	GENERATED_BODY()
	
public:
	// Sets default values for this character's properties
	ASoccerDemon();
	void EnableChaseAttackCollision(const bool Enabled);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
		void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void ShootAttack() override;

	UPROPERTY(EditAnywhere, Category = "Chase Collider")
		class UBoxComponent* ChaseCollider;

	UPROPERTY(EditDefaultsOnly, Category = "Projectile")
		TSubclassOf<AProjectile> ProjectileType;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
		class USceneComponent* ProjectileSpawnPoint;

private:

#pragma region Chase Attack Parameters

	UPROPERTY(EditAnywhere, Category = "Soccer Demon|Chase Attack", meta = (UIMin = 0.0, UIMax = 10000.0))
		float ChaseAttack_ChaseSpeed = 4000.0f;

	UPROPERTY(EditAnywhere, Category = "Soccer Demon|Chase Attack", meta = (UIMin = 0.0, UIMax = 100.0))
		float ChaseAttack_DamageAmount = 30.0f;

	UPROPERTY(EditAnywhere, Category = "Soccer Demon|Chase Attack", meta = (UIMin = 1000.0, UIMax = 5000.0))
		float ChaseAttack_KnockbackDistance = 1000.0f;

#pragma endregion Chase Attack Parameters

};
