// Fill out your copyright notice in the Description page of Project Settings.

#include "Enemy.h"
#include "HellOfARacketCharacter.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BrainComponent.h"

// Sets default values
AEnemy::AEnemy()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

/// <summary>
///
///		Overrides Actor class "TakeDamage" function.
///		The Damage Taken from the Player toward the Enemy. Damage should be dealt by Player.
///
/// </summary>
float AEnemy::TakeDamage(float DamageAmount, FDamageEvent const &DamageEvent, AController *EventInstigator, AActor *DamageCauser)
{
	float DamageTaken = 0;
	if (!IsDead())
	{
		/*StopTree();*/
		DamageTaken = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
		DamageTaken = FMath::Min(CurrentHealth, DamageTaken);
		CurrentHealth -= DamageTaken;
		//UE_LOG(LogTemp, Warning, TEXT("Enemy Health Left: %f"), CurrentHealth);

		// Knockback Enemy
		if (DamageCauser != nullptr)
		{
			AHellOfARacketCharacter* PlayerCharacter = Cast<AHellOfARacketCharacter>(DamageCauser);
			if (PlayerCharacter != nullptr)
			{
				const float KnockBackForce = 1000.0f;
				FVector LaunchDir = KnockBackForce * FVector(
					PlayerCharacter->GetCamRotVec().X,
					PlayerCharacter->GetCamRotVec().Y,
					0.45f);
			}
		}
	}
	
	if (IsDead())
	{
		DropItem(GetActorLocation(), GetActorRotation());
		GiveKillScore();
		DestroyActorAndWeapons();
	}

	return DamageTaken;
}

/// <summary>
///
///		This function acts as a "Blackhole" in which Projectiles get sucked into when the player reflects projectiles back at Enemies.
///		Essentially, the Collider that uses this overlap begin collision as a delegate should ONLY be enemies.
///
///		This is meant to aid the player in reflecting projectiles as it can be quite hard to make shots while on the move.
///
/// </summary>
void AEnemy::OnOverlapBegin_ProjectileLockOn(class UPrimitiveComponent *OverlappedComponent, class AActor *OtherActor, class UPrimitiveComponent *OtherComp,
											 int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult, float DeltaTime)
{
	// Finding the distance between the projectile and enemy
	AProjectile *Projectile = Cast<AProjectile>(OtherActor);
	if (Projectile == nullptr)
		return;

	const AActor *ProjectileOwner = IReflectable::Execute_GetReflectOwner(Projectile);
	if (ProjectileOwner == nullptr)
		return;

	if (this != OtherActor && Projectile->GetIsReflected() && OtherActor == Projectile)
	{
		// Become Homing
		float PullIntensity = Projectile->GetProjMovement()->Velocity.Length() / 50.0f;

		// Pull Projectile Toward Enemy
		const float Distance = FVector::Distance(Projectile->GetActorLocation(), OtherActor->GetActorLocation());
		const FVector &CurrentEnemyLocation = FMath::VInterpTo(Projectile->GetActorLocation(), GetActorLocation(), DeltaTime, PullIntensity);
		Projectile->SetActorLocation(CurrentEnemyLocation);
	}
}

/// <summary>
///		Self-Explanatory
/// </summary>
void AEnemy::SetSpeed(const float Speed)
{
	GetCharacterMovement()->MaxWalkSpeed = Speed;
}

/// <summary>
///
///		Checks if the Current Enemy is dead. So basic functionality is if the enemy's health is depleted.
///		Should be used inside of Enemy::TakeDamage function only.
///
/// </summary>
bool AEnemy::IsDead() const
{
	return CurrentHealth <= 0;
}

// This function is meant to be used by different enemies that want to shoot a projectile at the player
void AEnemy::ShootAttack() {}

/// <summary>
///
///		This function is responsible for calculating the initial velocity of the projectile to achieve an 'arc' affect.
///		NOTE: This will be used for Enemies that use Projectiles in their attacks.
///
/// </summary>
FVector AEnemy::GetProjectileStartVelocity(const FVector &StartPosition, FVector PlayerPosition, const FVector &PlayerVelocity) const
{
	FVector HorizontalVector = PlayerPosition + (PlayerVelocity / 1.5f) - StartPosition;
	HorizontalVector.Z = 0.0f;
	HorizontalVector.Normalize();
	HorizontalVector = HorizontalVector * ProjectileSpeed;

	FVector PlayerHorizontal = FVector(PlayerPosition.X, PlayerPosition.Y, 0.0f);
	FVector StartHorizontal = FVector(StartPosition.X, StartPosition.Y, 0.0f);

	float TimeOfFlight = FVector::Distance(PlayerHorizontal, StartHorizontal) / HorizontalVector.Length();
	float height = StartPosition.Z - PlayerPosition.Z; // to aim lower, we could subtract from here...

	PlayerPosition.Z = 0;
	float VerticalVelocity = ((0.5) * 980 * FMath::Square(TimeOfFlight) + (PlayerPosition.Z - height)) / TimeOfFlight;
	FVector FinalVector = FVector(HorizontalVector.X, HorizontalVector.Y, VerticalVelocity);

	return FinalVector;
}

/// <summary>
///
///		This method is responsible for dropping health kits at a given location and rotation
///
/// </summary>
void AEnemy::DropItem(const FVector &Location, const FRotator &Rotation)
{
	// Randomly drop a number of items upon death
	int16 RandomDropAmount = round(rand() % (MaxItemDrop - MinItemDrop + 1) + MinItemDrop);
	FVector DemonLocation = GetActorLocation();
	FRotator DemonRotation = GetActorRotation();

	SpawnDrop(DemonLocation, DemonRotation, RandomDropAmount);
}

/// <summary>
///
///		This method is only to be used by the Enemy::DropItem function.
///		It randomly chooses a spawn point in a circle within the location's initial
///		Random Rotation in the world.
///
/// </summary>
void AEnemy::SpawnDrop(const FVector &Location, const FRotator &Rotation, const int16 NumOfDrops)
{
	FActorSpawnParameters SpawnParams;

	for (int16 i = 0; i < NumOfDrops; i++)
	{
		const FRotator& RandomRotation = Rotation * rand();

		// Grab some random food drop for variety reasons. Also, perform a check in case health kit items don't exist to avoid a crash.
		TSubclassOf<AActor> HealthKitItemDrop = nullptr;
		if (!HealthKitItems.IsEmpty())
		{
			int16 HealthKitNum = HealthKitItems.Num();
			int16 RandomHealthKitIndex = FMath::RandRange(0, HealthKitNum - 1);
			HealthKitItemDrop = HealthKitItems[RandomHealthKitIndex];
		}

		if (killedByMelee && TennisBallItemDrop != nullptr)
			ATennisBallPickup *SpawnActor = GetWorld()->SpawnActor<ATennisBallPickup>(TennisBallItemDrop, Location, RandomRotation, SpawnParams);

		else if (!killedByMelee && HealthKitItemDrop != nullptr)
			AHealthKit *SpawnActor = GetWorld()->SpawnActor<AHealthKit>(HealthKitItemDrop, Location, RandomRotation, SpawnParams);
	}
}

/// <summary>
///
///		Gives player score based on kill (melee or reflection)
///
/// </summary>
void AEnemy::GiveKillScore() const
{
	const int score = (killedByMelee) ? MeleeKillScore : ReflectionKillScore;
	const FString reason = "Kill";
	AHellOfARacketCharacter *PlayerCharacter = Cast<AHellOfARacketCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (PlayerCharacter != nullptr)
		PlayerCharacter->GetScoreComponent()->AddToScore(score, reason);
}

void AEnemy::DestroyActorAndWeapons()
{
	TArray<AActor *> AttachedActors;
	GetAttachedActors(AttachedActors);

	if (AttachedActors.Num() > 0)
	{

		for (AActor *Actor : AttachedActors)
		{
			Actor->Destroy();
		}
	}
	Destroy();
}
