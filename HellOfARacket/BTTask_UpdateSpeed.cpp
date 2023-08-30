// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_UpdateSpeed.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "FootballDemon.h"

UBTTask_UpdateSpeed::UBTTask_UpdateSpeed()
{
	NodeName = TEXT("UpdateSpeed");
}

EBTNodeResult::Type UBTTask_UpdateSpeed::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	if (OwnerComp.GetAIOwner() != nullptr)
	{
		AEnemy* EnemyCharacter = Cast<AEnemy>(OwnerComp.GetAIOwner()->GetPawn());

		if (EnemyCharacter != nullptr)
			EnemyCharacter->SetSpeed(OwnerComp.GetBlackboardComponent()->GetValueAsFloat(GetSelectedBlackboardKey()));

		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}