// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Components/InputComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/TimelineComponent.h"
#include "Components/PostProcessComponent.h"
#include "Engine/World.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "Reflectable.h"
#include "ScoringComponent.h"
#include "CamShake_MeleeHit.h"
#include "CamShake_OnTakeDamage.h"
#include "Enemy.h"


#include "HellOfARacketCharacter.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FGameOver);

UCLASS()
class HELLOFARACKET_API AHellOfARacketCharacter : public ACharacter
{
	GENERATED_BODY()

protected:
	// On Begin Play Event
	virtual void BeginPlay() override;

public:
	// Constructor
	AHellOfARacketCharacter();

	// Tick
	virtual void Tick(float DeltaTime) override;

	// Player Landed on Ground Event
	virtual void Landed(const FHitResult& Hit) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


	UPROPERTY(BlueprintAssignable)
		FGameOver GameOverEvent;
	
	/* --------------- Public Player Functions --------------- */
#pragma region Public Methods

	UFUNCTION(BlueprintCallable)
		void Reflect(const float Damage);

	void ShakeCamera(const TSubclassOf<UMatineeCameraShake>& CameraShakeType);

	/* ----- Health/Life/Damage Methods ----- */
	#pragma region Health/Life/Damage Methods
		bool IsDead() const;
		void AddHealth(const float HealthAmount);
		virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
		void ResetInvincibilityFrames();
		UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Health")
			void Respawn();
	#pragma endregion Health/Life/Damage Methods

	/* ----- TennisBall Powerup Methods ----- */
	#pragma region TennisBall Powerup Methods
		void SetBall();
		void RemoveBall();
		bool GetBall();
	#pragma endregion TennisBall Powerup Methods

	/* ----- Sound Methods ----- */
	#pragma region Sound Methods
		UFUNCTION(BlueprintImplementableEvent, Category = "SoundSystem")
			void playDashSound();
		UFUNCTION(BlueprintImplementableEvent, Category = "SoundSystem")
			void playJumpSound();
		UFUNCTION(BlueprintImplementableEvent, Category = "SoundSystem")
			void playDamageSound();
		UFUNCTION(BlueprintImplementableEvent, Category = "SoundSystem")
			void playHealthUpSound();
		UFUNCTION(BlueprintImplementableEvent, Category = "SoundSystem")
			void playHitSound();
	#pragma endregion Sound Methods

	/* ----- PowerUp Methods ----- */
	#pragma region PowerUp Methods
		UFUNCTION(BlueprintPure, Category = "Power")
			FName GetCurrentPowerUp() const;
		UFUNCTION(BlueprintCallable, Category = "Power")
			void SetCurrentPowerUp(const FName& NewPowerUp);
		UFUNCTION(BlueprintImplementableEvent)
			void DispatchActivateToFMOD(const FName& ActivatedPowerup);
		UFUNCTION(BlueprintCallable, Category = "PowerUps")
			void EndCurrentPowerUp();
		UFUNCTION(BlueprintCallable, Category = "PowerUps")
			float GetTimeFromTimeHandle();
		UFUNCTION(BlueprintImplementableEvent)
			void SwapBigRacket();
		UFUNCTION(BlueprintImplementableEvent)
			void SwapSmallRacket();
	#pragma endregion PowerUp Methods

	/* ----- Extra Getter Methods ----- */
	#pragma region Extra Getter Methods
		UFUNCTION(BlueprintCallable)
			UScoringComponent* GetScoreComponent() const;
		UFUNCTION(BlueprintCallable)
			FVector GetCamRotVec();
		UFUNCTION(BlueprintPure)
			FVector2D CalcProjectileRingStartSize() const;
		UFUNCTION(BlueprintPure)
			FVector2D CalcProjectileDistanceToUIRing(const float X, const float Y, const bool IsVisible);
	#pragma endregion Extra Getter Methods

#pragma endregion Public Methods

	/* --------------- Public Fields Fields --------------- */
#pragma region Public Fields

	/* ----- Health and Life Fields ----- */
	#pragma region Health and Lives Fields

		// Health Variables
		UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Hell Of A Racket Character|Health", meta = (ToolTip = "The Max Health of the Player. Default is 100"))
			float MaxHealth = 100.0f;

		UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Hell Of A Racket Character|Health", meta = (ToolTip = "The Current Health of the Player. Recommended that it's equal to Max Health by default."))
			float CurrentHealth = MaxHealth;

		UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Hell Of A Racket Character|Health", meta = (ToolTip = "The Amount of time (seconds) when player cannot receive damage. This is active when the player takes damage already."))
			float InvincibilityTime = 2.0f;

		UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Hell Of A Racket Character|Health", meta = (ToolTip = "The number of Lives the Player has on Start."))
			int Lives = 3;

		UPROPERTY(BluePrintReadWrite, EditAnywhere, Category = "Hell Of A Racket Character|Health", meta = (ToolTip = "The number of Lives the Player has on Start."))
			bool Respawning = false;


	#pragma endregion Health and Life Fields

	/* ----- Camera Effect Fields ----- */
	#pragma region Camera Effect Fields

		UPROPERTY(BluePrintReadWrite, EditAnywhere, Category = "Hell Of A Racket Character|Camera", meta = (ToolTip = "The camera 'Field Of View' for the Player (Default value is 90)."))
			float DefaultFOV;

		UPROPERTY(EditAnywhere, Category = "Hell Of A Racket Character|Camera|Movement Tilt", meta = (ToolTip = "While moving left, Camera will tilt by this amount. Recommended Values should be negative."))
			float CameraTiltMin = -1.5f;

		UPROPERTY(EditAnywhere, Category = "Hell Of A Racket Character|Camera|Movement Tilt", meta = (ToolTip = "While moving right, Camera will tilt by this amount. Recommended Values should be positive."))
			float CameraTiltMax = 1.5f;

		UPROPERTY(EditAnywhere, Category = "Hell Of A Racket Character|Camera|WallRun Tilt", meta = (ToolTip = "While wall running left, Camera will tilt by this amount. Recommended Values should be negative."))
			float WallRunCameraTiltMin = -4.0f;

		UPROPERTY(EditAnywhere, Category = "Hell Of A Racket Character|Camera|WallRun Tilt", meta = (ToolTip = "While wall running right, Camera will tilt by this amount. Recommended Values should be positive."))
			float WallRunCameraTiltMax = 4.0f;

		UPROPERTY(EditAnywhere, Category = "Hell Of A Racket Character|Camera|Reticle", meta = (ToolTip = "X and Y size for the reticle ring at Maximum projectile distance."))
			float MaxRingSize = 500.0f;

		UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Hell Of A Racket Character|Camera|Shake")
			bool CamShakeEnabled = true;

		UPROPERTY(EditAnywhere, Category = "Hell Of A Racket Character|Camera|Shake")
			TSubclassOf<UMatineeCameraShake> MeleeHitShake;

		UPROPERTY(EditAnywhere, Category = "Hell Of A Racket Character|Camera|Shake")
			TSubclassOf<UMatineeCameraShake> TakeDamageShake;

		UPROPERTY(EditAnywhere, Category = "Hell Of A Racket Character|Camera|Shake")
			TSubclassOf<UMatineeCameraShake> OnHitGroundShake;

		UPROPERTY()
			float DefaultPostProcess;

	#pragma endregion Camera Effect Fields

	/* ----- Movement Fields ----- */
	#pragma region Movement Fields
		UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Hell Of A Racket Character|Movement", meta = (ToolTip = "The Z Velocity of the Pink Launcher Orbs' Launch Vector."))
			float PinkLauncherZVelocity;

		UPROPERTY(BlueprintReadOnly)
			float HorizontalMoveValue;

		UPROPERTY(BlueprintReadOnly)
			float VerticalMoveValue;

		UPROPERTY(BlueprintReadWrite)
			bool Jumping;

		UPROPERTY(BlueprintReadWrite)
			bool WallRunning;

		UPROPERTY(BlueprintReadWrite)
			bool IsHoldingJumpButton = false;

		UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Hell Of A Racket Character|Movement", meta = (ToolTip = "The Player Move Speed."))
			float DefaultMaxWalkSpeed = 2000.0f;

		UPROPERTY()
			float GroundFricConst;

	#pragma endregion Movement Fields

	/* ----- Dash Fields ----- */
	#pragma region Dash Fields

		UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Hell Of A Racket Character|Movement|Dash", meta = (ToolTip = "The number of maximum dashes the player has at all times."))
			int DefaultMaxDashes = 3;

		UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Hell Of A Racket Character|Movement|Dash")
			int MaxDashes = 3;

		UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Hell Of A Racket Character|Movement|Dash", meta = (ToolTip = "The current number of dashes the player has. Not Editable through Editor, but it really shouldn't be."))
			int CurrentDashes;

		UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Hell Of A Racket Character|Movement|Dash")
			float DashDistance;

		UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Hell Of A Racket Character|Movement|Dash", meta = (ToolTip = "The distance covered by performing a dash (both in air and grounded)."))
			float DefaultDashDistance = 3500.0f;

		UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Hell Of A Racket Character|Movement|Dash", meta = (ToolTip = "While in air, the amount of force needed to dash the player (Default value is 10)."))
			float InAirDashForce = 10.0f;

		UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Hell Of A Racket Character|Movement|Dash", meta = (ToolTip = "The amount of time (in seconds) required before player can perform a dash"))
			float DashCooldownTime;

		UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Hell Of A Racket Character|Movement|Dash", meta = (ToolTip = "The amount of time (in seconds) taken to restore a dash."))
			float DashReplenishTime;

		UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Hell Of A Racket Character|Movement|Dash", meta = (ToolTip = "The amount of time (in seconds) taken to restore a dash."))
			float DefaultDashReplenishTime = 2.0f;

		UPROPERTY(BlueprintReadWrite)
			float DashFrictionWaitTime;
		
		UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Hell Of A Racket Character|Movement|Dash", meta = (ToolTip = "Controls wether the player can dash"))
			bool CanDash = true;
		

	#pragma endregion Dash Fields

	/* ----- Swing-Action Fields ----- */
	#pragma region Swing-Action Fields

		UPROPERTY(BlueprintReadWrite)
			float SwingCooldownTimer;

		UPROPERTY(BlueprintReadWrite)
			float MaxSwingChargeTime;

		UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Hell Of A Racket Character|Combat|Swing", meta = (ToolTip = "The amount of time (in seconds) required before player can swing (Default is 0.35 seconds)."))
			float DefaultSwingCooldownTimer = 0.35f;

		UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Hell Of A Racket Character|Combat|Swing", meta = (ToolTip = "The amount of time (in seconds) required for the player to fully charge their swing (Default is 2.5 seconds)."))
			float DefaultMaxSwingChargeTime = 2.5f;

		UPROPERTY(BlueprintReadOnly)
			float ChargeRealTimeCounter;

		UPROPERTY(BlueprintReadOnly)
			bool AttackButtonHeld = false;

	#pragma endregion Swing-Action Fields

	/* ----- Melee Fields ----- */
	#pragma region Melee Fields

		UPROPERTY(BlueprintReadWrite)
			float MeleeRange;

		UPROPERTY(BlueprintReadWrite)
			float MinMeleeDamage;

		UPROPERTY(BlueprintReadWrite)
			float MaxMeleeDamage;

		UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Hell Of A Racket Character|Combat|Melee", meta = (ToolTip = "The range in which the player can melee hit enemies within. IN CENTIMETERS."))
			float DefaultMeleeRange = 1000.0f;

		UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Hell Of A Racket Character|Combat|Melee", meta = (ToolTip = "The minimum melee damage. Typically, this value is met when the player's Swing Charge Time is '0'."))
			float DefaultMinMeleeDamage = 10.0f;

		UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Hell Of A Racket Character|Combat|Melee", meta = (ToolTip = "The maximum melee damage. Typically, this value is met when the player's Swing Charge Time is 'Max Swing Charge Time'."))
			float DefaultMaxMeleeDamage = 50.0f;

	#pragma endregion Melee Fields

	/* ----- Reflection Fields ----- */
	#pragma region Reflection Fields

		UPROPERTY(BlueprintReadWrite)
			float ReflectionRange;

		UPROPERTY(BlueprintReadWrite)
			float ReflectionHalfAngle;

		UPROPERTY(BlueprintReadWrite)
			float MinReflectionDamage;

		UPROPERTY(BlueprintReadWrite)
			float MaxReflectionDamage;

		UPROPERTY(BlueprintReadWrite)
			float MinReflectionForce;

		UPROPERTY(BlueprintReadWrite)
			float MaxReflectionForce;

		UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Hell Of A Racket Character|Combat|Reflection", meta = (UIMin = 0.1f, UIMax = 1.0f))
			float InRangeReflectionTimeSpeed = 1.0f;

		UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Hell Of A Racket Character|Combat|Reflection", meta = (ToolTip = "The range in which the player can reflect projectiles within."))
			float DefaultReflectionRange = 1000.0f;

		UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Hell Of A Racket Character|Combat|Reflection", meta = (ToolTip = "The length (in cm) added to the reflection range vector used when determining the maximum start distance for the reticle range indicator."))
			float ViewingRangeModifier = 1000.0f;

		UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Hell Of A Racket Character|Combat|Reflection", meta = (ToolTip = "Angle in which player can reflect within. Values greater than this will not allow reflection."))
			float DefaultReflectionHalfAngle = 45.0f;

		UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Hell Of A Racket Character|Combat|Reflection", meta = (ToolTip = "The minimum reflection damage. Typically, this value is met when the player's Swing Charge Time is '0'."))
			float DefaultMinReflectionDamage = 10.0f;

		UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Hell Of A Racket Character|Combat|Reflection", meta = (ToolTip = "The maximum reflection damage. Typically, this value is met when the player's Swing Charge Time is 'Max Swing Charge Time'."))
			float DefaultMaxReflectionDamage = 50.0f;

		UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Hell Of A Racket Character|Combat|Reflection", meta = (ToolTip = "The minimum reflection force applied when reflecting projectiles. Typically, this value is met when the player's Swing Charge Time is '0'."))
			float DefaultMinReflectionForce = 100.0f;

		UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Hell Of A Racket Character|Combat|Reflection", meta = (ToolTip = "The maximum reflection force applied when reflecting projectiles. Typically, this value is met when the player's Swing Charge Time is 'Max Swing Charge Time'."))
			float DefaultMaxReflectionForce = 500.0f;

		UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Hell Of A Racket Character|Combat|Reflection")
			float HorizontalTargetAngle;

		UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Hell Of A Racket Character|Combat|Reflection")
			float VerticalTargetAngle;

	#pragma endregion Reflection Fields

	/* ----- Self-Knockback Fields ----- */
	#pragma region Self-Knockback Fields

		UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Hell Of A Racket Character|Combat|Self-Knockback", meta = (ToolTip = "The minimum backwards knockback force applied to the player when causing melee damage. Typically, this value is met when the player's Swing Charge Time is '0'."))
			float MinNormalRacketKnockback = 500.0f;

		UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Hell Of A Racket Character|Combat|Self-Knockback", meta = (ToolTip = "The maximum backwards knockback force applied to the player when causing melee damage. Typically, this value is met when the player's Swing Charge Time is 'Max Swing Charge Time'."))
			float MaxNormalRacketKnockback = 800.0f;

		UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Hell Of A Racket Character|Combat|Self-Knockback", meta = (ToolTip = "The minimum backwards knockback force applied to the player while holding 'Big Racket' PowerUp. Typically, this value is met when the player's Swing Charge Time is '0'."))
			float MinBigRacketKnockback = 1000.0f;

		UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Hell Of A Racket Character|Combat|Self-Knockback", meta = (ToolTip = "The maximum backwards knockback force applied to the player while holding 'Big Racket' PowerUp. Typically, this value is met when the player's Swing Charge Time is 'Max Swing Charge Time'."))
			float MaxBigRacketKnockback = 3000.0f;

		UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Hell Of A Racket Character|Combat|Self-Knockback", meta = (ToolTip = "The minimum backwards knockback force applied to the player when performing reflections. Typically, this value is met when the player's Swing Charge Time is '0'."))
			float MinReflectionKnockback = 500.0f;

		UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Hell Of A Racket Character|Combat|Self-Knockback", meta = (ToolTip = "The maximum backwards knockback force applied to the player when performing reflections. Typically, this value is met when the player's Swing Charge Time is 'Max Swing Charge Time'."))
			float MaxReflectionKnockback = 1000.0f;

	#pragma endregion Self-Knockback Fields

	/* ----- TennisBall Powerup Fields ----- */
	#pragma region TennisBall Powerup Fields

		UPROPERTY(EditDefaultsOnly, Category = "Hell Of A Racket Character|Projectile", meta = (ToolTip = "The TennisBall object to Spawn when player obtains a TennisBall PickUp."))
			TSubclassOf<class AProjectile> TennisBallClass;

		UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
			class USceneComponent* TennisBallSpawnPoint;

		UPROPERTY(BlueprintReadOnly)
			bool HasBall = false;

	#pragma endregion TennisBall Powerup Fields

		/* ----- Miscellaneous ----- */
	#pragma region Misc

		UPROPERTY(BlueprintReadOnly)
			bool InRangeRed = false;

		UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "PowerUps")
			float PowerupDuration = 15.0f;


	#pragma endregion Misc

#pragma endregion Public Fields

private:
	/* --------------- Private Player Functions --------------- */
#pragma region Private Methods

	/* ----- Attack Methods ----- */
	#pragma region Attack Methods

		void Attack();
		void Melee(const float DamageAmount);
		void Serve();
		void ChargeSwing();
		float GetTimeChargeAttackHeld() const;
		void ApplySelfKnockback(const bool IsMeleeHit);
		void ApplyBigRacketKnockback();
		void ResetReflect();
		void ResetMelee();
		void PinkLaunch();

	#pragma endregion Attack Methods

	/* ----- Movement Methods ----- */
	#pragma region Movement Methods

		void HorizontalMove(const float Value);
		void VerticalMove(const float Value);
		void StartJump();
		void EndJump();
		UFUNCTION(BlueprintPure)
			float GetHorizontalMovementDirection() const;
		UFUNCTION(BlueprintPure)
			float GetVerticalMovementDirection() const;

	#pragma endregion Movement Methods

	/* ----- Camera Control Methods  ----- */
	#pragma region Camera Control Methods 

		void HorizontalRotation(const float Value);
		void VerticalRotation(const float Value);
		void UpdateCameraFOV(float DeltaTime);
		void UpdatePostProcess(float DeltaTime);
		void CameraFreezeEffect(const float TargetGameSpeed);
		UFUNCTION(BlueprintCallable, Category = "Tilting")
			void TiltCamera(const float Value);

	#pragma endregion Camera Control Methods 

	/* ----- Dash Methods ----- */
	#pragma region Dash Methods

		void Dash();
		void PropelPlayer(const FVector& Dir, const float Value);
		void ReplenishDash();
		void ResetDashCooldown();
		void ResetDashFriction();
		UFUNCTION(BlueprintCallable)
			float GetDashCooldown();

	#pragma endregion Dash Methods

	/* ----- Extra Helper Methods ----- */
	#pragma region Extra Helper Methods

		FVector GetRandomDirection(float vertAngle, float horizAngle, FVector start);
		const float GetAngleBetweenCenter(const FVector& CenterView, const FVector& ToOther);
		float CalculateChargedValueBetween(const float MinValue, const float MaxValue) const;
		void SetInGameTimeStep(const float Speed);
		FVector2D ResetReticleSize() const;

	#pragma endregion Extra Helper Methods

#pragma endregion Private Methods
	
	/* --------------- Private Player Fields --------------- */
#pragma region Private Fields

	/* ----- Player Control Fields ----- */
	#pragma region Player Control Fields

		UPROPERTY()
			bool TookDamage;

		UPROPERTY()
			bool HitGround;

		UPROPERTY()
			bool MidAirSwingUsed;

		UPROPERTY()
			bool Dashing;

		UPROPERTY()
			bool DashCooling;

		UPROPERTY()
			bool Reflecting;
	
		UPROPERTY()
			bool Meleeing;

		UPROPERTY()
			FName CurrentPowerUp;

	#pragma endregion Player Control Fields

	/* ----- Components ----- */
	#pragma region Component Fields

		UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
			class UCameraComponent* Cam;

		UPROPERTY(EditAnywhere)
			class UTimelineComponent* CameraTimeline;

		UPROPERTY(EditAnywhere)
			class UPostProcessComponent* PostProcess;

		UPROPERTY(EditAnywhere)
			class UScoringComponent* Score;

	#pragma endregion Component Fields

	/* ----- Score Fields ----- */
	#pragma region Score Fields

		int score = 0;

		int storedScore = 0;

		int multiplier = 1;

	#pragma endregion Score Fields

	/* ----- Timer Handlers ----- */
	#pragma region Timer Handlers

		FTimerHandle ResetDamageFrameHandle;
		FTimerHandle ChargeTimerHandle;
		FTimerHandle ReplenishDashHandle;
		FTimerHandle ResetDashFricHandle;
		FTimerHandle DashCooldownHandle;
		FTimerHandle ResetReflectHandle;
		FTimerHandle ResetMeleeHandle;
		FTimerHandle ReflectCooldownHandle;
		FTimerHandle PowerupTimerHandle;

	#pragma endregion Timer Handlers

#pragma endregion Private Fields

};




