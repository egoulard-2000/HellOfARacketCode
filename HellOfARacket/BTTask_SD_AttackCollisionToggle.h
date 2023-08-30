// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_SD_AttackCollisionToggle.generated.h"


UCLASS()
class HELLOFARACKET_API UBTTask_SD_AttackCollisionToggle : public UBTTaskNode
{
	GENERATED_BODY()

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

public:
	UBTTask_SD_AttackCollisionToggle();

private:

	UPROPERTY(EditAnywhere)
		bool AttackCollisionEnabled = false;
};
