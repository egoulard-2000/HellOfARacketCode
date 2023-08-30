// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "EnemyInterface.generated.h"

/*
	
	This class is no longer supported, please derive from Enemy.cpp class instead. Thamk yoi!!! :)

*/
UINTERFACE(meta = (DeprecatedFunction, DeprecationMessage = "Interface has been deprecated, Please use the Enemy.cpp class instead"), MinimalAPI)
class UEnemyInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class HELLOFARACKET_API IEnemyInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(meta = (DeprecatedFunction, DeprecationMessage = "Function has been deprecated, Please use the new function in Enemy.cpp"), BlueprintNativeEvent, Category = "Enemy")
		bool IsDead() const;

	UFUNCTION(meta = (DeprecatedFunction, DeprecationMessage = "Function has been deprecated, Please use the new function in Enemy.cpp"), BlueprintNativeEvent, Category = "Enemy")
		void Attack();

	UFUNCTION(meta = (DeprecatedFunction, DeprecationMessage = "Function has been deprecated, Please use the new function in Enemy.cpp"), BlueprintNativeEvent, Category = "Enemy")
		void DropHealthKit();

};
