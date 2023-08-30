// Fill out your copyright notice in the Description page of Project Settings.


#include "ScoringComponent.h"

UScoringComponent::UScoringComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UScoringComponent::BeginPlay()
{
	Super::BeginPlay();
	
}


// Called every frame
void UScoringComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	SaveConfig();
}

// Add a given score to the value for a given reason
void UScoringComponent::AddToScore(int scoreIn, FString reason) {

	this->preMultScore += scoreIn;

	PreScoreChange.Broadcast(preMultScore);


	// Change this, may need other reasons and/or better data types for switch. 
	if (reason.Equals("Kill")) {
		currentKillStreakMult = currentKillStreakMult == 0 ? killStreakMultiplier : currentKillStreakMult * killStreakGrowthFactor;
	}

	int scoreDiff = preMultScore - currentThreshold;

	if (scoreDiff > MultiplierThreshold) {
		AddToMultiplier(scoreDiff / MultiplierThreshold);
		MultChange.Broadcast(multiplier);
	}
}

void UScoringComponent::AddToMultiplier(int mult) {

	this->multiplier += mult;
	
	if (multiplier > multCap) multiplier = multCap;

	GetOwner()->GetWorldTimerManager().SetTimer(multTimer, this, &UScoringComponent::ResetMultiplier, multResetTime, false);
	MultChange.Broadcast(multiplier);
}

void UScoringComponent::ResetMultiplier() {

	this->score += (this->multiplier + this->currentKillStreakMult) * this->preMultScore;

	this->multiplier = 1;

	this->currentKillStreakMult = 0;
	this->preMultScore = 0;
	

	ScoreChange.Broadcast(score);
	MultChange.Broadcast(multiplier);
	PreScoreChange.Broadcast(preMultScore);


}

int UScoringComponent::GetScore() const {
	return score;
}
int UScoringComponent::GetCurrentPreMultScore() const {
	return preMultScore;
}
float UScoringComponent::GetMultiplier() const {
	return multiplier;
}

void UScoringComponent::Jump() {
	inAir = true;
}

void UScoringComponent::Landed() {
	inAir = false;
}




