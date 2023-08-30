// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Reflectable.h"
#include "Projectile.h"
#include "Kismet/GameplayStatics.h"

#include "BossReflector.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class HELLOFARACKET_API UBossReflector : public USceneComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UBossReflector();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Hit Bounds")
		float reflectRadius;

	// How many times the projectile can be reflected before the boss will take damage instead
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Reflection")
		float maxNumReflect;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Reflection")
		float reflectSpeed;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Reflection")
		float damage;

private:
	UPROPERTY(EditAnywhere)
		class UCapsuleComponent* ReflectorCollision;

	UPROPERTY(EditAnywhere)
		int timesReflected;

	UPROPERTY(EditAnywhere)
		class AProjectile* mostRecentProj;

	UPROPERTY(EditAnywhere)
		class AActor* parentActor;

public:
	AProjectile* getMostRecentProj();

private:
	bool isProjectile(AActor* actor);
	bool isReflectable(AActor* actor);
	void onOverlap(AActor* actor);
	void reflectTowardPlayer(AProjectile* proj);
};
