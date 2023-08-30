// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "SoccerDemonAIController.generated.h"

/**
 * 
 */
UCLASS()
class HELLOFARACKET_API ASoccerDemonAIController : public AAIController
{
	GENERATED_BODY()
	
public:
	virtual void Tick(float DeltaSeconds) override;

protected:
	virtual void BeginPlay() override;

	void InitializeKeys();

private:

	UPROPERTY(EditAnywhere)
		float WalkSpeed = 500.0f;

	UPROPERTY(EditAnywhere)
		float ChaseSpeed = 4000.0f;

	UPROPERTY(EditAnywhere)
		float EnemyAggroRange = 10000.0f;

	UPROPERTY(EditAnywhere)
		float PatrolRadius = 3000.0f;

	// The Behavior Tree of this AI Controller
	UPROPERTY(EditAnywhere)
		class UBehaviorTree* SoccerDemonAIBehavior;

	FTimerHandle KeyHandle;
};
