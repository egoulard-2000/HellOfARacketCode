// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Reflectable.generated.h"

// This class does not need to be modified.
UINTERFACE(Blueprintable, BlueprintType)
class UReflectable : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class HELLOFARACKET_API IReflectable
{
	GENERATED_BODY()

public:

	// Function Defined by the Reflected Projectile/Enemy/Whatever
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		void Reflect(FVector direction, float force, int damage, AActor* initiator);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		AActor* GetReflectOwner();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		void SetReflectOwner(AActor* owner);

};
