// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/World.h"

#include "ScoringComponent.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FScoreChange, int, score);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), Config=Game)
class HELLOFARACKET_API UScoringComponent : public UActorComponent
{
	GENERATED_BODY()


	//TODO:
		// 360 No-scope - Implementation is dependant on many factors.
		// OOB kill
public:	
	// Sets default values for this component's properties
	UScoringComponent();


protected:

	
	// Called when the game starts
	virtual void BeginPlay() override;

	int score = 0;
	int preMultScore = 0;
	int currentThreshold = 0;
	float multiplier = 1;

	

	float currentKillStreakMult = 0.0;


	bool inAir = false;
	bool wallRunning = false;

	FTimerHandle multTimer;



public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


	// -- Blueprint Editable Properties --

	UPROPERTY(BlueprintReadWrite, Config)
		int MultiplierThreshold = 1000;
	
	UPROPERTY(BlueprintReadWrite, Config)
		float dashScoreCooldown = 0.5f;

	UPROPERTY(BlueprintReadWrite, Config)
		float dashScoreMultiplier = 0.5f;

	UPROPERTY(BlueprintReadWrite, Config)
		float multResetTime = 1.0;

	UPROPERTY(BlueprintReadWrite, Config)
		float killStreakMultiplier = 0.1f;

	UPROPERTY(BlueprintReadWrite, Config)
		float killStreakGrowthFactor = 2.0f;

	UPROPERTY(BlueprintReadWrite, Config)
		float multCap = 10.0;

	
	UPROPERTY(BlueprintAssignable)
		FScoreChange ScoreChange;

	UPROPERTY(BlueprintAssignable)
		FScoreChange MultChange;

	UPROPERTY(BlueprintAssignable)
		FScoreChange PreScoreChange;

	// -- Functions --

	UFUNCTION(BlueprintCallable)
		void AddToScore(int score, FString reason);
	UFUNCTION(BlueprintCallable)
		void AddToMultiplier(int mult);
	UFUNCTION(BlueprintCallable)
		int GetScore() const;
	UFUNCTION(BlueprintCallable)
		int GetCurrentPreMultScore() const;
	UFUNCTION(BlueprintCallable)
		float GetMultiplier() const;
	UFUNCTION(BlueprintCallable)
		void ResetMultiplier();

	UFUNCTION()
		void Jump();
	UFUNCTION()
		void Landed();
	//TODO Highscores
	//TODO Score Triggered Multipliers
	//TODO Score Multiplier Decay


};
