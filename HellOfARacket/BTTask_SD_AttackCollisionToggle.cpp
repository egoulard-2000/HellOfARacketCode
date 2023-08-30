// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_SD_AttackCollisionToggle.h"
#include "AIController.h"
#include "SoccerDemon.h"

UBTTask_SD_AttackCollisionToggle::UBTTask_SD_AttackCollisionToggle()
{
	NodeName = TEXT("Chase Attack Collision Toggle");
}

EBTNodeResult::Type UBTTask_SD_AttackCollisionToggle::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	if (OwnerComp.GetAIOwner() != nullptr)
	{
		ASoccerDemon* SoccerDemonCharacter = Cast<ASoccerDemon>(OwnerComp.GetAIOwner()->GetPawn());
		if (SoccerDemonCharacter != nullptr)
			SoccerDemonCharacter->EnableChaseAttackCollision(AttackCollisionEnabled);

		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}