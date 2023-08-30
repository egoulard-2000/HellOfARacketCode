// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_FocusOnPlayer.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"


UBTTask_FocusOnPlayer::UBTTask_FocusOnPlayer()
{
	NodeName = TEXT("BTTask_FocusOnPlayer");
}

EBTNodeResult::Type UBTTask_FocusOnPlayer::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	if (OwnerComp.GetAIOwner() != nullptr)
	{
		AActor* PlayerPawn = Cast<AActor>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
        AAIController* AIController = Cast<AAIController>(OwnerComp.GetAIOwner());
        
        if (PlayerPawn == nullptr)
            return EBTNodeResult::Failed; 

        AIController->SetFocus(PlayerPawn);

		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}