#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Reflectable.h"
#include "FloatingFireball.generated.h"

UCLASS()
class HELLOFARACKET_API AFloatingFireball : public AActor, public IReflectable
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AFloatingFireball();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

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

	UPROPERTY(EditAnywhere)
		float LifeSpan = 5.0f;

	UPROPERTY(EditAnywhere)
		float DamageAmount = 30.0f;

	UFUNCTION(BlueprintCallable)
		void FireInDirection(const FVector& direction);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		void Reflect(FVector direction, float force, int damage, AActor* initiator);

	UFUNCTION(BlueprintCallable)
		void Bounce(const FHitResult& r, const FVector& ImpactVelocity);

	UFUNCTION(BlueprintCallable)
		void RefreshBounces();

	UFUNCTION(BlueprintCallable)
		void OnHit(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
