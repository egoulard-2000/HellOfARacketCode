// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Kismet/GameplayStatics.h"
#include "PickleBallSatanAIController.generated.h"

/**
 * 
 */
UCLASS()
class HELLOFARACKET_API APickleBallSatanAIController : public AAIController
{
	GENERATED_BODY()
	
public:
	virtual void Tick(float DeltaSeconds) override;

protected:
	virtual void BeginPlay() override;

	void InitializeKeys();

private:

	UPROPERTY(EditAnywhere)
		float WalkSpeed = 200.0f;

	UPROPERTY(EditAnywhere)
		float RunSpeed = 700.0f;

	UPROPERTY(EditAnywhere)
		float EnemyAggroRange = 1200.0f;

	UPROPERTY(EditAnywhere)
		float PatrolRadius = 3000.0f;

	UPROPERTY(EditAnywhere)
		FName Name = "Pickleball Satan";

	// The Behavior Tree of this AI Controller
	UPROPERTY(EditAnywhere)
		class UBehaviorTree* PickleBallSatanAIBehavior;

	FTimerHandle KeyHandle;
};
