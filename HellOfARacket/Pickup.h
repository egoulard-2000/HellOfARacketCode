// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Pickup.generated.h"

UCLASS(Abstract, Blueprintable)
class HELLOFARACKET_API APickup : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APickup();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
		void ItemInterp(float DeltaTime, float Value);

	UFUNCTION()
		void AnimatePickup(float DeltaTime);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void ActivatePickup();

	UFUNCTION()
		void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

private:
	UPROPERTY()
		bool IsCollide = false;

	UPROPERTY()
		FVector StartLocation;

	UPROPERTY()
		FVector EndLocation;

	UPROPERTY(EditAnywhere)
		float PickupRange = 300.0f;

	UPROPERTY(EditAnywhere, Category = "Item")
		class USphereComponent* PickupRangeTrigger;

#pragma region Animation

	UPROPERTY(EditAnywhere, Category = "Movement")
		float XValue;

	UPROPERTY(EditAnywhere, Category = "Movement")
		float YValue;

	UPROPERTY(EditAnywhere, Category = "Movement")
		float ZValue = 25.0f;

	UPROPERTY(EditAnywhere, Category = "Movement")
		float PitchValue;

	UPROPERTY(EditAnywhere, Category = "Movement")
		float YawValue = 2.5f;

	UPROPERTY(EditAnywhere, Category = "Movement")
		float RollValue;

	float RunningTime;

#pragma endregion Animation

	FTimerHandle InterpTimer;
};
