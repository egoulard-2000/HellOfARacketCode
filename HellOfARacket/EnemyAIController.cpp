// Fill out your copyright notice in the Description page of Project Settings.



#include "EnemyAIController.h"
#include "Kismet/GameplayStatics.h"

#include "BehaviorTree/BlackboardComponent.h"


void AEnemyAIController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

void AEnemyAIController::BeginPlay()
{
	Super::BeginPlay();

	// Run the Enemy AI Behavior Tree upon playing
	if (EnemyAIBehavior != nullptr)
		RunBehaviorTree(EnemyAIBehavior);
}
