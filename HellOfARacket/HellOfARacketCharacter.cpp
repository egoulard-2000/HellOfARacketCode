// Fill out your copyright notice in the Description page of Project Settings.

#include "HellOfARacketCharacter.h"
#include "PowerUp.h"
#include "Projectile.h"

// Sets default values
AHellOfARacketCharacter::AHellOfARacketCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	TennisBallSpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("SpawnPoint"));
	TennisBallSpawnPoint->SetupAttachment(RootComponent);

	//Immediately make this FP Character assigned to player0
	AutoPossessPlayer = EAutoReceiveInput::Player0;

#pragma region Dash Variables

	DashDistance = DefaultDashDistance;
	CurrentDashes = MaxDashes;
	DashCooldownTime = 0.5f;
	DashReplenishTime = 2.0f;
	DashFrictionWaitTime = 0.3f;
	DashCooling = false;
	GroundFricConst = GetCharacterMovement()->GroundFriction;

#pragma endregion Dash Variables

#pragma region Player/Camera Variables
	
	Jumping = false;
	WallRunning = false;
	HitGround = true;

	HorizontalMoveValue = 0.0f;
	VerticalMoveValue = 0.0f;

	Cam = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Cam->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform); //attaches camera to center of player and to follow player
	Cam->SetRelativeLocation(FVector(0, 0, 40)); //adjust camera to go up by 40 in z-axis

	Score = CreateDefaultSubobject<UScoringComponent>(TEXT("Score"));

	DefaultFOV = Cam->FieldOfView;

	PostProcess = CreateDefaultSubobject<UPostProcessComponent>(TEXT("PostProcess"));
	PostProcess->BlendWeight = 0.0f;
	DefaultPostProcess = PostProcess->BlendWeight;

#pragma endregion Player/Camera Variables

}

// Called when the game starts or when spawned
void AHellOfARacketCharacter::BeginPlay()
{
	Super::BeginPlay();

#pragma region Swing/Melee/Reflection Variables

	SwingCooldownTimer = DefaultSwingCooldownTimer;
	MaxSwingChargeTime = DefaultMaxSwingChargeTime;

	MeleeRange = DefaultMeleeRange;
	MinMeleeDamage = DefaultMinMeleeDamage;
	MaxMeleeDamage = DefaultMaxMeleeDamage;

	ReflectionRange = DefaultReflectionRange;
	ReflectionHalfAngle = DefaultReflectionHalfAngle;

	MinReflectionDamage = DefaultMinReflectionDamage;
	MaxReflectionDamage = DefaultMaxReflectionDamage;

	MinReflectionForce = DefaultMinReflectionForce;
	MaxReflectionForce = DefaultMaxReflectionForce;

	CurrentPowerUp = FName(TEXT("NoPowerUp"));

#pragma endregion Swing/Melee/Reflection Variables

}

// Called every frame
void AHellOfARacketCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

#pragma region Check Player Grounded

	if (GetCharacterMovement()->IsFalling())
		HitGround = false;

#pragma endregion Check Player Grounded

#pragma region Check Real Time Charge Timer

	if (AttackButtonHeld && !Meleeing)
	{
		ChargeRealTimeCounter += DeltaTime;
		ChargeRealTimeCounter = GetTimeChargeAttackHeld();
	}

#pragma endregion Check Real Time Charge Timer

#pragma region Check Camera Motions

	// Check FPCamera Motions
	UpdateCameraFOV(DeltaTime);
	UpdatePostProcess(DeltaTime);

#pragma endregion Check Camera Motions

#pragma region Check Dashing Motions

	Dashing = false; //This is here to ensure the post-processing doesn't always show up
	if (!GetCharacterMovement()->IsFalling() && HitGround)
		GetCharacterMovement()->FallingLateralFriction = 0.0f;

#pragma endregion Check Dashing Motion

}

// Called to bind functionality to player input
void AHellOfARacketCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	//Binds the control mappings from input in project settings to player controller input
	InputComponent->BindAxis("Horizontal", this, &AHellOfARacketCharacter::HorizontalMove);
	InputComponent->BindAxis("Vertical", this, &AHellOfARacketCharacter::VerticalMove);

	//InputComponent->BindAxis("HorizontalRotation", this, &AHellOfARacketCharacter::HorizontalRotation);
	//InputComponent->BindAxis("VerticalRotation", this, &AHellOfARacketCharacter::VerticalRotation);

	InputComponent->BindAction("Jump", IE_Pressed, this, &AHellOfARacketCharacter::StartJump);
	InputComponent->BindAction("Jump", IE_Released, this, &AHellOfARacketCharacter::EndJump);

	InputComponent->BindAction("Attack", IE_Pressed, this, &AHellOfARacketCharacter::ChargeSwing);
	InputComponent->BindAction("Attack", IE_Released, this, &AHellOfARacketCharacter::Attack);

	InputComponent->BindAction("Serve", IE_Pressed, this, &AHellOfARacketCharacter::Serve);

	InputComponent->BindAction("Dash", IE_Pressed, this, &AHellOfARacketCharacter::Dash);
}

#pragma region Player Health/Damage Methods

float AHellOfARacketCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (!TookDamage)
	{
		TookDamage = true;
		GetWorldTimerManager().SetTimer(ResetDamageFrameHandle, this, &AHellOfARacketCharacter::ResetInvincibilityFrames, InvincibilityTime, false);

		float DamageTaken = 0;
		DamageTaken = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
		DamageTaken = FMath::Min(CurrentHealth, DamageTaken);
		CurrentHealth -= DamageTaken;
		InRangeRed = false;

		ShakeCamera(TakeDamageShake);
		playDamageSound();

		if (IsDead() && !Respawning)
		{
			Respawning = true;
			Lives--;
			if (Lives == 0)
			{
				GameOverEvent.Broadcast();
			}
			else
			{
				Respawn();
			}
		}
		return DamageTaken;
	}

	return 0.0f;
}

bool AHellOfARacketCharacter::IsDead() const
{
	return CurrentHealth <= 0;
}

void AHellOfARacketCharacter::AddHealth(const float HealthAmount)
{
	// Add Health, but make sure it doesn't go above the Max Health
	if (this != nullptr)
	{
		CurrentHealth += HealthAmount;
		CurrentHealth = FMath::Min(MaxHealth, CurrentHealth);
	}
	playHealthUpSound();
}

void AHellOfARacketCharacter::ResetInvincibilityFrames()
{
	TookDamage = false;
	GetWorldTimerManager().ClearTimer(ResetDamageFrameHandle);
}

#pragma endregion Player Health/Damage Methods

#pragma region Player/Camera Movement Functions

void AHellOfARacketCharacter::HorizontalMove(const float Value)
{
	HorizontalMoveValue = Value;
	TiltCamera(Value); //Always update camera tilt

	if (Value) { AddMovementInput(GetActorRightVector(), Value); }
}

void AHellOfARacketCharacter::VerticalMove(const float Value)
{
	VerticalMoveValue = Value;

	if (Value) { AddMovementInput(GetActorForwardVector(), Value); }
}

//void AHellOfARacketCharacter::HorizontalRotation(const float Value)
//{
//	AddControllerYawInput(Value * HorizontalMouseSensitivity);
//}
//
//void AHellOfARacketCharacter::VerticalRotation(const float Value)
//{
//	AddControllerPitchInput(-Value * VerticalMouseSensitivity);
//}

float AHellOfARacketCharacter::GetHorizontalMovementDirection() const
{
	return HorizontalMoveValue;
}

float AHellOfARacketCharacter::GetVerticalMovementDirection() const
{
	return VerticalMoveValue;
}

UScoringComponent* AHellOfARacketCharacter::GetScoreComponent() const
{
	return Score;
}

#pragma endregion Player Camera/Movement Functions

#pragma region Combat Functions

// Initiates an "Attack" Action by the player. This consists of Melee or Reflect Attacks.
void AHellOfARacketCharacter::Attack()
{
	AttackButtonHeld = false;

	// Calculate the Damage associated with Melee and Reflection as well as Big Racket Knockback
	const float MeleeDamage = CalculateChargedValueBetween(MinMeleeDamage, MaxMeleeDamage);
	UE_LOG(LogTemp, Warning, TEXT("Melee Damage: %f"), MeleeDamage);
	const float ReflectionDamage = CalculateChargedValueBetween(MinReflectionDamage, MaxReflectionDamage);
	UE_LOG(LogTemp, Warning, TEXT("Reflection Damage: %f"), ReflectionDamage);

	// Calculate Reflect and Melee Based on Damage (taking into account Charge Attack Time Held)
	ApplyBigRacketKnockback();
	Reflect(ReflectionDamage);
	Melee(MeleeDamage);
}


// Called When 'Attack' Action is Pressed
void AHellOfARacketCharacter::Melee(const float DamageAmount)
{
	// Cannot Melee Until Swing Cooldown Complete
	if (!Meleeing)
	{
		// Player Can Now Melee Attack, but must wait "swingCoolDownTimer" seconds before next swing
		Meleeing = true;
		GetWorldTimerManager().SetTimer(ResetMeleeHandle, this, &AHellOfARacketCharacter::ResetMelee, SwingCooldownTimer, false);

		UE_LOG(LogTemp, Warning, TEXT("Melee Attacking"));

		// Params for capturing a 'hit' on line trace
		FVector Start = Cam->GetComponentLocation();
		FVector End = Start + Cam->GetForwardVector() * MeleeRange; // 'meleeRange' is the length of the vector (centimeters) --Hi. ben here. holy shit i did not know you were using cm as the unit lmao
		FHitResult Hit;
		FCollisionQueryParams Params;
		Params.AddIgnoredActor(this);

		// If an actor is hit by line trace (in front of camera)
		bool hit = GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECollisionChannel::ECC_WorldStatic, Params);
		if (hit)
		{
			#pragma region nullptr checks
			AActor* HitActor = Hit.GetActor();

			AEnemy* Enemy = Cast<AEnemy>(HitActor);
			if (Enemy == nullptr) { return; }

			AController* PlayerController = GetController();
			if (PlayerController == nullptr) { return; }
			#pragma endregion nullptr checks

			// Perform Damage on Hit Enemy
			Enemy->killedByMelee = true;
			UE_LOG(LogTemp, Warning, TEXT("HIT ACTOR"));
			const FVector& HitDirection = -Cam->GetComponentRotation().Vector();
			FPointDamageEvent DamageEvent(DamageAmount, Hit, HitDirection, nullptr);

			HitActor->TakeDamage(DamageAmount, DamageEvent, PlayerController, this); // This will be called in Enemy Class
			ShakeCamera(MeleeHitShake);
			ApplySelfKnockback(true);
			playHitSound();
			if (ChargeRealTimeCounter != MaxSwingChargeTime)
				CameraFreezeEffect(0.75f);
			else
				CameraFreezeEffect(0.55f);
		}
	}
}

//called to launch the player if they hit a pink launcher with a melee attack.
void AHellOfARacketCharacter::PinkLaunch()
{
	FVector pinkLaunchVector = FVector(0, 0, PinkLauncherZVelocity) ;
	LaunchCharacter(pinkLaunchVector, false, true);
	Jumping = true;
} 

void AHellOfARacketCharacter::Reflect(const float DamageAmount)
{
	// Cannot Reflect Until Swing Cooldown Complete
	if (!Reflecting)
	{
		// Player Can Now Reflect, but must wait "swingCoolDownTimer" seconds before next swing
		Reflecting = true;
		GetWorldTimerManager().SetTimer(ResetReflectHandle, this, &AHellOfARacketCharacter::ResetReflect, SwingCooldownTimer, false);

		// Setup 
		bool Reflected = false;
		bool First = true;
		FVector StartDir = Cam->GetForwardVector();

		// Scale Reflection Force based on charged swing longevity
		const float ReflectionForce = CalculateChargedValueBetween(MinReflectionForce, MaxReflectionForce);

		// Get all Reflectable Actors Overlapping Player's Reflector
		TArray<AActor*> ReflectableActors;
		UGameplayStatics::GetAllActorsWithInterface(GetWorld(), UReflectable::StaticClass(), ReflectableActors);

		for (AActor* Actor : ReflectableActors)
		{
			// Compute the Direction, Distance, and Angle of Reflectable Objects
			FVector ProjDir = Actor->GetActorLocation() - this->GetActorLocation();
			float Distance = abs(ProjDir.Length());
			float Angle = abs(GetAngleBetweenCenter(StartDir, ProjDir));

			if (Distance > ReflectionRange || Angle > ReflectionHalfAngle) continue;

			FVector ReflectDir = StartDir;
			Reflected = true;
			IReflectable::Execute_SetReflectOwner(Actor, this);

			// Is First Projectile Reflected?
			if (First)
			{
				First = false;

				TArray<AActor*> Enemies;
				UGameplayStatics::GetAllActorsOfClass(GetWorld(), AEnemy::StaticClass(), Enemies);

				for (AActor* Enemy : Enemies)
				{
					FVector Dir = Enemy->GetActorLocation() - GetActorLocation();
					const float AngleHere = GetAngleBetweenCenter(StartDir, Dir);
					float BestAngle = HorizontalTargetAngle;

					if (AngleHere < BestAngle)
					{
						BestAngle = AngleHere;
						Dir.Normalize();
						ReflectDir = Dir;
					}
				}
			}
			else
			{
				ReflectDir = GetRandomDirection(HorizontalTargetAngle, VerticalTargetAngle, ReflectDir);
			}

			IReflectable::Execute_Reflect(Actor, ReflectDir, ReflectionForce, DamageAmount, this);
			ApplySelfKnockback(false);
			if(ChargeRealTimeCounter != MaxSwingChargeTime)
				CameraFreezeEffect(0.30f);
			else
				CameraFreezeEffect(0.15f);
		} 
	}
}

// Self-Explanatory. 
// Takes in whether the registered hit was a melee hit (true) or reflection hit (false)
void AHellOfARacketCharacter::ApplySelfKnockback(const bool IsMeleeHit)
{
	const FVector& MeleeKnockbackDir = Cam->GetComponentRotation().Vector() * -CalculateChargedValueBetween(MinNormalRacketKnockback, MaxNormalRacketKnockback);
	const FVector& ReflectKnockbackDir = Cam->GetComponentRotation().Vector() * -CalculateChargedValueBetween(MinReflectionKnockback, MaxReflectionKnockback);
	const FVector& BackwardDir = IsMeleeHit ? MeleeKnockbackDir : ReflectKnockbackDir;

	LaunchCharacter(BackwardDir, true, true);
}

// Self-Explanatory.
void AHellOfARacketCharacter::ApplyBigRacketKnockback()
{
	// Launches Player backwards and only usable once in midair
	if (CurrentPowerUp == "BigRacket")
	{
		if (!MidAirSwingUsed)
		{
			const FVector& BackwardDir = Cam->GetComponentRotation().Vector() * -((MinBigRacketKnockback + MaxBigRacketKnockback) / 2);
			LaunchCharacter(BackwardDir, true, true);
			MidAirSwingUsed = true;
		}
	}
}

void AHellOfARacketCharacter::Serve()
{
	if (HasBall)
	{
		UE_LOG(LogTemp, Warning, TEXT("Serving"));
		RemoveBall();

		APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(this, 0);

		if (PlayerPawn)
		{
			// Spawns the projectile actor
			AProjectile* SpawnedProjectile =
				GetWorld()->SpawnActor<AProjectile>(
					TennisBallClass,
					Cam->GetComponentLocation() + Cam->GetComponentRotation().Vector() * 100,
					Cam->GetComponentRotation());

			IReflectable::Execute_SetReflectOwner(SpawnedProjectile, this);
			SpawnedProjectile->SetIsReflected(true); // if true, it becomes a homing projectile when near enemies (false otherwise)
			SpawnedProjectile->GetProjMovement()->Velocity = SpawnedProjectile->GetActorForwardVector() * 7000.0f;
			ApplySelfKnockback(false);
			CameraFreezeEffect(0.95f);
		}
	}
}

// Calculate the amount of time the Charge Attack was held in seconds
float AHellOfARacketCharacter::GetTimeChargeAttackHeld() const
{
	const float CurrentEstimatedTimeHeld = FMath::Min(ChargeRealTimeCounter, MaxSwingChargeTime); // Only get Max Swing Charge Time unless the time held is shorter

	return CurrentEstimatedTimeHeld;
}

// A Helper Method that returns the calculated Value between Min and Max Values based on the Time Scalar
float AHellOfARacketCharacter::CalculateChargedValueBetween(const float MinValue, const float MaxValue) const
{
	const float TimeHeld = GetTimeChargeAttackHeld();

	const float CalculatedValue = MinValue + ((MaxValue - MinValue) * TimeHeld);
	const float CurrentValue = FMath::Floor(FMath::Min(MaxValue, CalculatedValue)); // Only get Max Value if Charge Attack held for long time.

	return CurrentValue;
}

FVector AHellOfARacketCharacter::GetRandomDirection(float vertAngle, float horizAngle, FVector start) 
{
	double phiRot = FMath::RandRange(-vertAngle, vertAngle);
	double thetaRot = FMath::RandRange(-horizAngle, horizAngle);

	FVector out = start.RotateAngleAxis(phiRot, FVector(0, 1, 0));
	out = out.RotateAngleAxis(thetaRot, FVector(0, 0, 1));

	out.Normalize();

	return out;
}

const float AHellOfARacketCharacter::GetAngleBetweenCenter(const FVector& CenterView, const FVector& ToOther)
{
	float dot = CenterView.Dot(ToOther);
	float mag = ToOther.Size() + CenterView.Size();
	float rad = FMath::Acos(dot / mag);
	return FMath::RadiansToDegrees(rad);
}

void AHellOfARacketCharacter::ResetReflect()
{
	Reflecting = false;
	GetWorldTimerManager().ClearTimer(ResetReflectHandle);
}

void AHellOfARacketCharacter::ResetMelee()
{
	Meleeing = false;
	GetWorldTimerManager().ClearTimer(ResetMeleeHandle);
}

// Called when attack button is PRESSED
// See Attack Function when button is RELEASED, automatically.
void AHellOfARacketCharacter::ChargeSwing()
{
	// Start Basic Attack First, Ensuring the player has now pressed the button before initiating a charge swing. 
	// Essentially, when player taps button, the attack will be basic. Otherwise, holding the charge attack will commence.
	ChargeRealTimeCounter = 0.0f;
	if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed("Attack"))
		Attack();

	// Now Attack Button is considered held
	AttackButtonHeld = true;
}

#pragma endregion Combat Functions

#pragma region Jump Function

void AHellOfARacketCharacter::StartJump()
{
	IsHoldingJumpButton = true;
	if (HitGround)
	{
		Jump();
		Jumping = true;
		HitGround = false;
		playJumpSound();
	}
}

void AHellOfARacketCharacter::EndJump()
{
	Jumping = false;
	IsHoldingJumpButton = false;

	if (WallRunning)
		playJumpSound();
}

// For when the Player Lands on the Ground Upon First Contact
void AHellOfARacketCharacter::Landed(const FHitResult& Hit)
{
	Super::Landed(Hit);
	HitGround = true;
	Jumping = false;
	MidAirSwingUsed = false;

	// If Player hits ground at great height, then play a camera shake
	if (FMath::Abs(GetCharacterMovement()->Velocity.Z) >= 3500)
		ShakeCamera(OnHitGroundShake);
}	

#pragma endregion Jump Function

#pragma region Dash Functions

void AHellOfARacketCharacter::Dash()
{
	// If not allowed to dash, do nothing
	if (CurrentDashes == 0 || DashCooling || !CanDash) { return; }

	Dashing = true;
	DashCooling = true;
	CurrentDashes--;
	playDashSound();

	//Disable friction while mid-dash
	GetCharacterMovement()->GroundFriction = 0.0f;

	// X and Y Vectors
	const FVector& HorizontalDir = GetActorRightVector();
	const FVector& VerticalDir = GetActorForwardVector();

	if (VerticalMoveValue != 0 && HorizontalMoveValue != 0) // Diagonal Dashing
	{
		const FVector& ForwardRightDir = VerticalDir + HorizontalDir;
		const FVector& ForwardLeftDir = VerticalDir - HorizontalDir;
		const FVector& BackwardRightDir = HorizontalDir - VerticalDir;
		const FVector& BackwardLeftDir = -VerticalDir - HorizontalDir;

		if (VerticalMoveValue == 1 && HorizontalMoveValue == 1) { PropelPlayer(ForwardRightDir, 1.0f); }

		else if (VerticalMoveValue == 1 && HorizontalMoveValue == -1) { PropelPlayer(ForwardLeftDir, 1.0f); }

		else if (VerticalMoveValue == -1 && HorizontalMoveValue == 1) { PropelPlayer(BackwardRightDir, 1.0f); }

		else { PropelPlayer(BackwardLeftDir, 1.0f); }
	}
	else if (HorizontalMoveValue) //Dash in left or right direction
	{
		PropelPlayer(HorizontalDir, HorizontalMoveValue);
	}
	else if (VerticalMoveValue) //Dash in forward or backward direction
	{
		PropelPlayer(VerticalDir, VerticalMoveValue);
	}
	else //Default simply dash in current forward direction
	{
		PropelPlayer(VerticalDir, 1.0f);
	}

	// Turn Friction Back On After dashFrictionWaitTime seconds
	GetWorldTimerManager().SetTimer(ResetDashFricHandle, this, &AHellOfARacketCharacter::ResetDashFriction, DashFrictionWaitTime, false);

	// Starts short cooldown before player can dash again
	GetWorldTimerManager().SetTimer(DashCooldownHandle, this, &AHellOfARacketCharacter::ResetDashCooldown, DashCooldownTime, false);

	// Starts replenishing dashes if not already after 'dashReplenishTime' seconds
	if (!GetWorldTimerManager().IsTimerActive(ReplenishDashHandle))
		GetWorldTimerManager().SetTimer(ReplenishDashHandle, this, &AHellOfARacketCharacter::ReplenishDash, DashReplenishTime, false);
}

// Simple Helper for Determining which direction to dash the player towards
void AHellOfARacketCharacter::PropelPlayer(const FVector& Dir, const float Value)
{
	FVector LaunchDir = Value * (Dir * DashDistance * InAirDashForce);
	LaunchDir.Z = 0.0; // Ensure the player isn't dragged down by gravity during launch

	if (GetCharacterMovement()->IsFalling())
	{
		// I drastically increase the air friction temporarily to give the illusion of fast frantic dashing in air
		GetCharacterMovement()->FallingLateralFriction = InAirDashForce;
		LaunchCharacter(LaunchDir, true, true);

		// Minimize the distance the player goes off of slopes during collision with ground during air dash
		const float GroundStopFriction = 20.0f;
		GetCharacterMovement()->GroundFriction = GroundStopFriction;
	}
	else
	{
		// Dash will be much lighter on the ground, but we carry more momentum
		const float MomentumMultiplier = 1.3f;
		LaunchCharacter(LaunchDir / InAirDashForce * MomentumMultiplier, true, true);
	}
}

void AHellOfARacketCharacter::ReplenishDash()
{
	CurrentDashes++;

	GetWorldTimerManager().ClearTimer(ReplenishDashHandle);

	// If player still doesn't have 3 dashes, continue restoring next dash
	if (CurrentDashes < MaxDashes)
		GetWorldTimerManager().SetTimer(ReplenishDashHandle, this, &AHellOfARacketCharacter::ReplenishDash, DashReplenishTime, false);
}

// Used to turn ground friction back on after a dash
void AHellOfARacketCharacter::ResetDashFriction()
{
	// resets based on the constant value the ground friction is set at initially.
	GetCharacterMovement()->GroundFriction = GroundFricConst;

	GetWorldTimerManager().ClearTimer(ResetDashFricHandle);
}

// End cooldown on singular dash
void AHellOfARacketCharacter::ResetDashCooldown()
{
	GetWorldTimerManager().ClearTimer(DashCooldownHandle);
	DashCooling = false;
}

// Self-Explanatory
float AHellOfARacketCharacter::GetDashCooldown() 
{
	return GetWorldTimerManager().GetTimerElapsed(DashCooldownHandle);
}

#pragma endregion Dash Functions

#pragma region Update Camera Properties

void AHellOfARacketCharacter::TiltCamera(const float Value)
{
	#pragma region Camera Tilt Types
	const enum CameraTiltType
	{
		HorizontalGroundMovement = 1,
		HorizontalWallRunMovement = 2
	};
	#pragma endregion Camera Tilt Types
	const float CurrentRotation = Cam->GetRelativeRotation().Roll;
	const float TiltRate = 8.0f;
	float TiltStabilizationRate = 0.0f; // When no longer moving, camera should tilt back at normalized ratio so it's more consistent for players

	if (Value != 0 && !WallRunning) // Moving on the Ground
	{
		TiltStabilizationRate = CameraTiltMax;

		if (Value == HorizontalGroundMovement) // Moving Right
			if (CurrentRotation < CameraTiltMax)
				AddControllerRollInput(Value / TiltRate * TiltStabilizationRate); // Tilt Camera Right

		if (Value == -HorizontalGroundMovement) // Moving Left
			if (CurrentRotation > CameraTiltMin)
				AddControllerRollInput(Value / TiltRate * TiltStabilizationRate); // Tilt Camera Left
	}
	else if (WallRunning)
	{
		TiltStabilizationRate = WallRunCameraTiltMax;

		if (Value == HorizontalWallRunMovement) // Wall Running on Left Side
			if (CurrentRotation < WallRunCameraTiltMax)
				AddControllerRollInput(Value / TiltRate * TiltStabilizationRate); // Tilt Camera Right

		if (Value == -HorizontalWallRunMovement) // Wall Running on Right Side
			if (CurrentRotation > WallRunCameraTiltMin)
				AddControllerRollInput(Value / TiltRate * TiltStabilizationRate); // Tilt Camera Left
	}
	else
	{
		AddControllerRollInput(-CurrentRotation / TiltRate + TiltStabilizationRate);
	}
}

void AHellOfARacketCharacter::ShakeCamera(const TSubclassOf<UMatineeCameraShake>& CameraShakeType)
{
	if (CamShakeEnabled)
	{
		const float CameraShakeScale = 1.0f;
		GetWorld()->GetFirstPlayerController()->PlayerCameraManager->StartCameraShake(CameraShakeType, CameraShakeScale);
	}
}

void AHellOfARacketCharacter::UpdateCameraFOV(float DeltaTime)
{
	const float newFOV = DefaultFOV + 60.0f;
	const float targetFOV = Dashing ? newFOV : DefaultFOV;
	const float dashInterpSpeed = 10.0f;
	float currentFOV = FMath::FInterpTo(Cam->FieldOfView, targetFOV, DeltaTime, dashInterpSpeed);

	Cam->SetFieldOfView(currentFOV);
}

void AHellOfARacketCharacter::UpdatePostProcess(float DeltaTime)
{
	float newPostProcess = 1.0;
	float targetPostProcess = Dashing ? newPostProcess : DefaultPostProcess;
	float postProcessBlendSpeed = 30.0f;
	float currentPostProcess = FMath::FInterpTo(PostProcess->BlendWeight, targetPostProcess, DeltaTime, postProcessBlendSpeed);

	PostProcess->BlendWeight = currentPostProcess;
}

// This Function is responsible for 'Freezing Time'
// 'TargetGameSpeed' should be a value between 0.1 and 1
void AHellOfARacketCharacter::CameraFreezeEffect(const float TargetGameSpeed)
{
	// ----- Interpolate FOV
	const float NewFOV = DefaultFOV - 35.0f;
	const float TargetFOV = (TargetGameSpeed != 1.0f) ? NewFOV : DefaultFOV;
	const float FOVInterpSpeed = 30.0f;
	float currentFOV = FMath::FInterpTo(Cam->FieldOfView, TargetFOV, GetWorld()->DeltaRealTimeSeconds, FOVInterpSpeed);
	Cam->SetFieldOfView(currentFOV);

	// ----- Freeze Time
	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), TargetGameSpeed);

	// Delay the execution for Freeze Time Duration (in seconds)
	//float FreezeTimeDuration = TargetGameSpeed / 2.5f;
	float FreezeTimeDuration = 0.12f;
	FTimerHandle SlowMotionTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(SlowMotionTimerHandle, [this]()
		{
			// Reset the global time dilation to its default value
			UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1.0f);
		}, FreezeTimeDuration, false);
}

FVector AHellOfARacketCharacter::GetCamRotVec()
{
	return Cam->GetComponentRotation().Vector();
}

#pragma endregion Update Camera Functions

#pragma region PowerUp Functions

void AHellOfARacketCharacter::SetBall()
{
	if (this != nullptr) // Once we have the collision with tennis balls working for only colliding with the capsule component, then we won't need this if check anymore.
		HasBall = true;
}

void AHellOfARacketCharacter::RemoveBall()
{
	if (this != nullptr) // Once we have the collision with tennis balls working for only colliding with the capsule component, then we won't need this if check anymore.
		HasBall = false;
}

bool AHellOfARacketCharacter::GetBall()
{
	return HasBall;
}

FName AHellOfARacketCharacter::GetCurrentPowerUp() const
{
	return CurrentPowerUp;
}

void AHellOfARacketCharacter::EndCurrentPowerUp() 
{
	// GetCharacterMovement()->MaxWalkSpeed = DefaultMaxWalkSpeed;
	
	DashReplenishTime = DefaultDashReplenishTime;
	
	DashDistance = DefaultDashDistance;
	MaxDashes = DefaultMaxDashes;

	SwingCooldownTimer = DefaultSwingCooldownTimer;
	MaxSwingChargeTime = DefaultMaxSwingChargeTime;

	MeleeRange = DefaultMeleeRange;
	MinMeleeDamage = DefaultMinMeleeDamage;
	MaxMeleeDamage = DefaultMaxMeleeDamage;

	ReflectionRange = DefaultReflectionRange;
	ReflectionHalfAngle = DefaultReflectionHalfAngle;

	MinReflectionDamage = DefaultMinReflectionDamage;
	MaxReflectionDamage = DefaultMaxReflectionDamage;

	MinReflectionForce = DefaultMinReflectionForce;
	MaxReflectionForce = DefaultMaxReflectionForce;

	if (CurrentPowerUp == "BigRacket") 
	{
		SwapSmallRacket();
	}

	CurrentPowerUp = "NoPowerUp";
}

void AHellOfARacketCharacter::SetCurrentPowerUp(const FName& NewPowerUp)
{
	EndCurrentPowerUp();

	CurrentPowerUp = NewPowerUp;

	if (CurrentPowerUp == "BigRacket")
	{
		SwingCooldownTimer = DefaultSwingCooldownTimer * 2;
		ReflectionRange = DefaultReflectionRange * 2;
		MeleeRange = DefaultMeleeRange * 2;
		
		// 2x ball reflection speed
		MinReflectionForce = 2 * DefaultMinReflectionForce;
		MaxReflectionForce = 2 * DefaultMaxReflectionForce;

		SwapBigRacket();
	}
	else if (CurrentPowerUp == "SteelRacket")
	{
		SwingCooldownTimer = DefaultSwingCooldownTimer / 2;
		MinMeleeDamage = FLT_MAX;
		MaxMeleeDamage = FLT_MAX;
	}
	else if (CurrentPowerUp == "Cleats")
	{
		// increase movement speed
		// GetCharacterMovement()->MaxWalkSpeed = DefaultMaxWalkSpeed * 2;

		// decrease dash cooldown
		DashReplenishTime = DefaultDashReplenishTime / 2;
		
		// increase dash distance
		DashDistance = DefaultDashDistance * 1.5f;
	}
	DispatchActivateToFMOD(CurrentPowerUp);
	
	GetWorldTimerManager().SetTimer(PowerupTimerHandle, this, &AHellOfARacketCharacter::EndCurrentPowerUp, PowerupDuration, false);
}

float AHellOfARacketCharacter::GetTimeFromTimeHandle() {
	float TimeValue = 0.f;
	if (GetWorld()->GetTimerManager().GetTimerRemaining(PowerupTimerHandle)) {
		TimeValue = GetWorld()->GetTimerManager().GetTimerRemaining(PowerupTimerHandle);
		return TimeValue;
	}
	return -1.f;
}

#pragma endregion PowerUp Functions

#pragma region UI Functions

FVector2D AHellOfARacketCharacter::CalcProjectileRingStartSize() const
{
	const float X = 90.0f;
	const float Y = 90.0f;
	FVector2D StartSize = FVector2D(X, Y);

	return StartSize;
}

// This function is ONLY to be used for gathering the distance data needed for resizing the "Projectile Ring" on the UI.
// 
// It is also known as the worst code ever written by man. It is practically ducktaped together. I'm shocked and appalled that this even works.
// All I can say is, I'm sorry. I'm truly sorry for what I've created. 
// I hope no one has to look at this code.
// - Emile
FVector2D AHellOfARacketCharacter::CalcProjectileDistanceToUIRing(const float X, const float Y, const bool IsVisible)
{
	FVector2D NewReticleSize = FVector2D(X, Y);

	// Viewing Range is the distance of the reflection collision (with a little breathing room)
	const float ViewingRange = ReflectionRange + ViewingRangeModifier;
	const FVector& StartDir = Cam->GetForwardVector();

	// Get all Reflectable Actors Overlapping Player's Reflector
	TArray<AActor*> ReflectableActors;
	UGameplayStatics::GetAllActorsWithInterface(GetWorld(), UReflectable::StaticClass(), ReflectableActors);

	for (AActor* Actor : ReflectableActors)
	{
		AProjectile* Projectile = Cast<AProjectile>(Actor);
		if (Projectile == nullptr)
		{
			NewReticleSize = ResetReticleSize();
			continue;
		}

		// Compute the Direction, Distance, and Angle of Reflectable Objects
		const FVector& ProjDir = Projectile->GetActorLocation() - this->GetActorLocation();
		const float Distance = abs(ProjDir.Length());
		const float Angle = abs(GetAngleBetweenCenter(StartDir, ProjDir));

		const float CrosshairDesiredSizeX = 90.0f;
		const float CrosshairDesiredSizeY = 90.0f;

		// Ensure that the reticle is always hidden, if any projectile is not within range.
		if (Distance > ViewingRange || Angle > ReflectionHalfAngle || Projectile->GetIsReflected())
		{
			InRangeRed = false;
			NewReticleSize = ResetReticleSize();
			continue;
		}

		if ((Distance <= ViewingRange && Angle <= ReflectionHalfAngle)) // Continue Shrinking
		{
			if (IsVisible)
			{
				// NewReticleSize.X = X - (ViewingRange * 4 * (ProjectileActor->GetProjMovement()->Velocity.Length() / CrosshairDesiredSizeX) / Distance);
				NewReticleSize.X = CrosshairDesiredSizeX + (MaxRingSize * (Distance / ViewingRange));

				// NewReticleSize.Y = Y - (ViewingRange * 4 * (ProjectileActor->GetProjMovement()->Velocity.Length() / CrosshairDesiredSizeY) / Distance);
				NewReticleSize.Y = CrosshairDesiredSizeY + (MaxRingSize * (Distance / ViewingRange));
			}
			
			// Lock on to Player's Position
			FVector PlayerLocation = Cam->GetComponentLocation();
			PlayerLocation.Z -= 20.0f;

			// Calculate Lock-On Velocity
			FVector Direction = PlayerLocation - Projectile->GetActorLocation();
			Direction.Normalize();
			float Speed = GetCharacterMovement()->MaxWalkSpeed + 325.0f;
			FVector Velocity = Direction * Speed;

			Projectile->GetProjMovement()->Velocity = Velocity; // Set the new velocity for the projectile
			InRangeRed = Distance <= ReflectionRange; // Update Reticle Color
			break;
		}
		else
		{
			// This is just here in case the above cases fail miraculouly. Because I know it will. It always does. It never ends.
			NewReticleSize = ResetReticleSize();
			break;
		}
	}

	return NewReticleSize;
}

FVector2D AHellOfARacketCharacter::ResetReticleSize() const
{
	FVector2D DefaultReticleSize;
	DefaultReticleSize.X = 90.0f;
	DefaultReticleSize.Y = 90.0f;

	return DefaultReticleSize;
}


// This function is responsible for setting a game speed during each time step. Speed Values Smaller than 1 activate "Slow-Mo" effect.
// Speed Value of 1 is the default speed of the game.
void AHellOfARacketCharacter::SetInGameTimeStep(const float Speed)
{
	// UGameplayStatics::SetGlobalTimeDilation(GetWorld(), Speed);

	// Experiementing with Camera FOV during slowmo
	/*const float NewFOV = DefaultFOV - 40.0f;
	const float TargetFOV = (Speed != 1.0f) ? NewFOV : DefaultFOV;
	const float InterpSpeed = 5.0f;
	float currentFOV = FMath::FInterpTo(Cam->FieldOfView, TargetFOV, GetWorld()->DeltaRealTimeSeconds, InterpSpeed);

	Cam->SetFieldOfView(currentFOV);*/
}

#pragma endregion UI Functions