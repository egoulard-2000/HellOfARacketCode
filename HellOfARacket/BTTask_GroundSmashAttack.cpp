// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_GroundSmashAttack.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "FootballDemon.h"

UBTTask_GroundSmashAttack::UBTTask_GroundSmashAttack()
{
	NodeName = TEXT("BTTask_GroundSmashAttack");
}

EBTNodeResult::Type UBTTask_GroundSmashAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	if (OwnerComp.GetAIOwner() != nullptr)
	{
		AFootballDemon* FootballDemonCharacter = Cast<AFootballDemon>(OwnerComp.GetAIOwner()->GetPawn());

		if (FootballDemonCharacter != nullptr)
			FootballDemonCharacter->GroundSmashAttack();

		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}