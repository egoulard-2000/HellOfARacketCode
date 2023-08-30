// Fill out your copyright notice in the Description page of Project Settings.
#include "PickleBallSatanAIController.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"


void APickleBallSatanAIController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	// Continually grab player location
	APawn* PlayerCharacter = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	/*GetBlackboardComponent()->SetValueAsVector(TEXT("PlayerLocation"), PlayerCharacter->GetActorLocation());*/

}

void APickleBallSatanAIController::BeginPlay()
{
	Super::BeginPlay();

	// Run the Football Demon Enemy AI Behavior Tree upon playing
	if (PickleBallSatanAIBehavior != nullptr)
	{
		RunBehaviorTree(PickleBallSatanAIBehavior);

		GetWorld()->GetTimerManager().SetTimer(KeyHandle, this, &APickleBallSatanAIController::InitializeKeys, 0.1f, false);
	}
}

void APickleBallSatanAIController::InitializeKeys()
{
	APawn* PlayerCharacter = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);

	// Setting up BlackBoard Key Values
	if (GetPawn() != nullptr)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("I've been possessed"));
		GetBlackboardComponent()->SetValueAsVector(TEXT("PlayerLocation"), PlayerCharacter->GetActorLocation());
		GetBlackboardComponent()->SetValueAsName(TEXT("EnemyType"), Name);
		GetBlackboardComponent()->SetValueAsFloat(TEXT("PatrolRadius"), PatrolRadius);
		GetBlackboardComponent()->SetValueAsBool(TEXT("BB_RallyCounter"), true);

		//GetBlackboardComponent()->SetValueAsFloat(TEXT("WalkSpeed"), WalkSpeed);
		//GetBlackboardComponent()->SetValueAsFloat(TEXT("RunSpeed"), RunSpeed);
		GetBlackboardComponent()->SetValueAsVector(TEXT("StartLocation"), GetPawn()->GetActorLocation());

	}
}
