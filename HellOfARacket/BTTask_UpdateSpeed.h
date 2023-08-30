// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_UpdateSpeed.generated.h"

/**
 * 
 */
UCLASS()
class HELLOFARACKET_API UBTTask_UpdateSpeed : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
public:
	UBTTask_UpdateSpeed();

};
