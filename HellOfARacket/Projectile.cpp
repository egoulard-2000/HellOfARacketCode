// Fill out your copyright notice in the Description page of Project Settings. (No, not right now Unreal Engine 5 of Epic Games Incorporated. But thank you tho)

#include "Projectile.h"
#include "Enemy.h"
#include "Kismet/KismetMathLibrary.h"
#include "HellOfARacketCharacter.h"

// Sets default values
AProjectile::AProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	IsReflected = false;

	Collider = CreateDefaultSubobject<USphereComponent>(TEXT("Collider"));
	Collider->InitSphereRadius(20.0f);
	Collider->OnComponentHit.AddDynamic(this, &AProjectile::OnHit);

	RootComponent = Collider;

	ProjMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile Movement"));
	ProjMovement->SetUpdatedComponent(Collider);
	ProjMovement->InitialSpeed = 1000.0f;
	ProjMovement->MaxSpeed = 0.0f;
	ProjMovement->bRotationFollowsVelocity = true;
	ProjMovement->bShouldBounce = true;
	ProjMovement->Bounciness = 0.3f;
	ProjMovement->ProjectileGravityScale = 0.0f;

	ProjMovement->OnProjectileBounce.AddDynamic(this, &AProjectile::Bounce);

	// LifeSpanTest = 5.0f;
}

// Called when the game starts or when spawned
void AProjectile::BeginPlay()
{
	Super::BeginPlay();
	SetLifeSpan(TotalLifeSpan);
	LifeSpan = TotalLifeSpan;
}

// Called every frame
void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (LifeSpan <= 0){
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT("Sent to HandleProjectileDestroyed"));
		HandleProjectileDestroyed();
	}
	else
		LifeSpan = LifeSpan - DeltaTime;
}

void AProjectile::FireInDirection(const FVector& direction)
{
	ProjMovement->Velocity = direction * ProjMovement->InitialSpeed;
}

/// <summary>
///		When the player reflects a projectile, we begin the reflection process
/// </summary>
void AProjectile::Reflect_Implementation(FVector direction, float force, int damage, AActor* initiator)
{
	IsReflected = true;
	ProjMovement->Velocity = direction * force;
	RefreshBounces();

	FString output = "Reflected: Damage=";
	output += FString::FromInt(damage);
	//if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, output);
}


void AProjectile::Bounce(const FHitResult& r, const FVector& ImpactVelocity)
{
	Bounces += 1;
	if (Bounces >= MaxBounces)
		Destroy();
}

void AProjectile::RefreshBounces()
{
	Bounces = 0;
}

void AProjectile::RefreshLifespan()
{
	SetLifeSpan(TotalLifeSpan);
	LifeSpan = TotalLifeSpan;
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT("REFRESHING LIFESPAN"));
}


/// <summary>
///		On Hit, Deal Damage to the Player or Enemy Hit and then disappear.
/// </summary>
void AProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	FVector NormalImpuse, const FHitResult& Hit)
{
	ACharacter* PlayerCharacter = Cast<AHellOfARacketCharacter>(OtherActor);
	AEnemy* EnemyCharacter = Cast<AEnemy>(OtherActor);

	// We don't want enemies damaging other enemies. So this is why this is here
	if (!IsReflected && OtherActor == EnemyCharacter)
	{
		Destroy();
		return;
	}

	// Must be player or enemy who overlaps
	if (this != OtherActor && (PlayerCharacter || EnemyCharacter))
	{
		// Cause Damage and Get Destroyed chump
		FDamageEvent DamageEvent;

		if (EnemyCharacter != nullptr)
			EnemyCharacter->killedByMelee = false;
		
		//The Hit Actor will take Damage
		OtherActor->TakeDamage(DamageAmount, DamageEvent, nullptr, nullptr);

		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT("Handling Event"));
		HandleProjectileDestroyed();
		Destroy();
	}
}

bool AProjectile::GetIsReflected() const
{
	return IsReflected;
}

void AProjectile::SetIsReflected(bool _IsReflected) 
{ 
	IsReflected = _IsReflected; 
}

UProjectileMovementComponent* AProjectile::GetProjMovement() const
{
	return ProjMovement;
}

AActor* AProjectile::GetReflectOwner_Implementation()
{
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("getting owner"));
	return owner;
}

void AProjectile::SetReflectOwner_Implementation(AActor* newOwner)
{
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("setting owner"));

	owner = newOwner;
}


void AProjectile::HandleProjectileDestroyed()
{
	OnProjectileDestroyed.ExecuteIfBound(); 
}