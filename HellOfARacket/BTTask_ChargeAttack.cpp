
// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_ChargeAttack.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "FootballDemon.h"

UBTTask_ChargeAttack::UBTTask_ChargeAttack()
{
	NodeName = TEXT("BTTask_FDChargeAttack");
}

EBTNodeResult::Type UBTTask_ChargeAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	if (OwnerComp.GetAIOwner() != nullptr)
	{
		AFootballDemon* FootballDemonCharacter = Cast<AFootballDemon>(OwnerComp.GetAIOwner()->GetPawn());

		if (FootballDemonCharacter != nullptr)
			FootballDemonCharacter->ChargeAttack();

		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}
