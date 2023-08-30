// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Reflectable.h"
#include "Projectile.generated.h"

DECLARE_DELEGATE(FOnProjectileDestroyed)

UCLASS(BlueprintType)

class HELLOFARACKET_API AProjectile : public AActor, public IReflectable
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AProjectile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
		void HandleProjectileDestroyed();

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleDefaultsOnly, Category = Projectile)
		USphereComponent* Collider;

	UPROPERTY(VisibleAnywhere, Category = Movement)
		UProjectileMovementComponent* ProjMovement;

	UPROPERTY(EditAnywhere)
		int MaxBounces = 3;

	int Bounces = 0;

	UPROPERTY()
		float LifeSpan = 1.0f;
	UPROPERTY(EditAnywhere)
		float TotalLifeSpan = 2.0f;

	UPROPERTY(EditAnywhere)
		float DamageAmount = 30.0f;

	UFUNCTION(BlueprintCallable)
		void FireInDirection(const FVector& direction);

	UFUNCTION(BlueprintCallable)
		void Bounce(const FHitResult& r, const FVector& ImpactVelocity);

	UFUNCTION(BlueprintCallable)
		void RefreshBounces();

	UFUNCTION(BlueprintCallable)
		void RefreshLifespan();

	UFUNCTION(BlueprintCallable)
		void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
			FVector NormalImpuse, const FHitResult& Hit);

	UFUNCTION(BlueprintPure)
		bool GetIsReflected() const;

	UFUNCTION()
		void SetIsReflected(bool _IsReflected);

	UProjectileMovementComponent* GetProjMovement() const;

	// Reflectable Methods
	
		virtual void Reflect_Implementation(FVector direction, float force, int damage, AActor* initiator) override;
		virtual AActor* GetReflectOwner_Implementation() override;

		virtual void SetReflectOwner_Implementation(AActor* newOwner) override;

		FOnProjectileDestroyed OnProjectileDestroyed;

private:
	AActor* owner;

	FVector ClosestEnemyLocation;

	UPROPERTY()
		bool IsReflected;

	float LifeSpanTest;
};
