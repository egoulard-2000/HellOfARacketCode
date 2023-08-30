// Fill out your copyright notice in the Description page of Project Settings.


#include "CamShake_MeleeHit.h"

UCamShake_MeleeHit::UCamShake_MeleeHit()
{
	OscillationDuration = 0.10f;
	OscillationBlendInTime = 0.05;
	OscillationBlendOutTime = 0.05;

	// Pitch Shake
	RotOscillation.Pitch.Amplitude = FMath::RandRange(5.0f, 7.0f);
	RotOscillation.Pitch.Frequency = FMath::RandRange(15.0f, 20.0f);

	// Yaw Shake
	RotOscillation.Yaw.Amplitude = FMath::RandRange(5.0f, 7.0f);
	RotOscillation.Yaw.Frequency = FMath::RandRange(15.0f, 20.0f);
}
