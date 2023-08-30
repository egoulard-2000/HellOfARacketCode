// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "EnemyAIController.generated.h"

/**
 * 
 */
UCLASS()
class HELLOFARACKET_API AEnemyAIController : public AAIController
{
	GENERATED_BODY()

public:
	virtual void Tick(float DeltaSeconds) override;

protected:
	virtual void BeginPlay() override;

private:

	// The Behavior Tree of this AI Controller
	UPROPERTY(EditAnywhere)
		class UBehaviorTree* EnemyAIBehavior;

};
