// Fill out your copyright notice in the Description page of Project Settings.

#include "SoccerDemonAIController.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"

void ASoccerDemonAIController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	const APawn* PlayerCharacter = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);

	// Sees Player
	if (LineOfSightTo(PlayerCharacter) && (PlayerCharacter->GetActorLocation() - GetPawn()->GetActorLocation()).Size() < EnemyAggroRange)
	{
		GetBlackboardComponent()->SetValueAsVector(TEXT("PlayerLocation"), PlayerCharacter->GetActorLocation());
		GetBlackboardComponent()->SetValueAsVector(TEXT("LastKnownPlayerLocation"), PlayerCharacter->GetActorLocation());
	}
	else
	{
		GetBlackboardComponent()->ClearValue(TEXT("PlayerLocation"));
	}
}

void ASoccerDemonAIController::BeginPlay()
{
	Super::BeginPlay();

	// Run the Football Demon Enemy AI Behavior Tree upon playing
	if (SoccerDemonAIBehavior != nullptr)
	{
		RunBehaviorTree(SoccerDemonAIBehavior);

		// Need a timer here for initializing variables ahead of Spawn AI
		GetWorld()->GetTimerManager().SetTimer(KeyHandle, this, &ASoccerDemonAIController::InitializeKeys, 0.1f, false);
	}
}

void ASoccerDemonAIController::InitializeKeys()
{
	const APawn* PlayerCharacter = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);

	// Setting up BlackBoard Key Values
	if (GetPawn() != nullptr)
	{
		GetBlackboardComponent()->SetValueAsVector(TEXT("PlayerLocation"), PlayerCharacter->GetActorLocation());
		GetBlackboardComponent()->SetValueAsFloat(TEXT("PatrolRadius"), PatrolRadius);
		GetBlackboardComponent()->SetValueAsFloat(TEXT("WalkSpeed"), WalkSpeed);
		GetBlackboardComponent()->SetValueAsFloat(TEXT("ChaseSpeed"), ChaseSpeed);
		GetBlackboardComponent()->SetValueAsVector(TEXT("StartLocation"), GetPawn()->GetActorLocation());
	}
}