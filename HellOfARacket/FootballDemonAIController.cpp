// Fill out your copyright notice in the Description page of Project Settings.


#include "FootballDemonAIController.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"

void AFootballDemonAIController::Tick(float DeltaSeconds)
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

void AFootballDemonAIController::BeginPlay()
{
	Super::BeginPlay();

	// Run the Football Demon Enemy AI Behavior Tree upon playing
	if (FootballDemonAIBehavior != nullptr)
	{
		RunBehaviorTree(FootballDemonAIBehavior);

		// Need a timer here for initializing variables ahead of Spawn AI
		GetWorld()->GetTimerManager().SetTimer(KeyHandle, this, &AFootballDemonAIController::InitializeKeys, 0.1f, false);
	}
}

void AFootballDemonAIController::InitializeKeys()
{
	const APawn* PlayerCharacter = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);

	// Setting up BlackBoard Key Values
	if (GetPawn() != nullptr)
	{
		GetBlackboardComponent()->SetValueAsVector(TEXT("PlayerLocation"), PlayerCharacter->GetActorLocation());
		GetBlackboardComponent()->SetValueAsFloat(TEXT("PatrolRadius"), PatrolRadius);
		GetBlackboardComponent()->SetValueAsFloat(TEXT("WalkSpeed"), WalkSpeed);
		GetBlackboardComponent()->SetValueAsFloat(TEXT("RunSpeed"), RunSpeed);
		GetBlackboardComponent()->SetValueAsVector(TEXT("StartLocation"), GetPawn()->GetActorLocation());
	}
}
