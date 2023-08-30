#include "FloatingFireball.h"
#include "Enemy.h"
#include "HellOfARacketCharacter.h"


// Sets default values
AFloatingFireball::AFloatingFireball()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Collider = CreateDefaultSubobject<USphereComponent>(TEXT("Collider"));
	Collider->InitSphereRadius(20.0f);
	Collider->OnComponentBeginOverlap.AddDynamic(this, &AFloatingFireball::OnHit);

	RootComponent = Collider;

	ProjMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile Movement"));
	ProjMovement->SetUpdatedComponent(Collider);
	ProjMovement->InitialSpeed = 100.0f;
	ProjMovement->MaxSpeed = 100.0f;
	ProjMovement->bRotationFollowsVelocity = true;
	ProjMovement->bShouldBounce = true;
	ProjMovement->Bounciness = 0.3f;
	ProjMovement->ProjectileGravityScale = 0.0f;

	ProjMovement->OnProjectileBounce.AddDynamic(this, &AFloatingFireball::Bounce);

}

// Called when the game starts or when spawned
void AFloatingFireball::BeginPlay()
{
	Super::BeginPlay();
	SetLifeSpan(LifeSpan);
}

// Called every frame
void AFloatingFireball::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AFloatingFireball::FireInDirection(const FVector& direction)
{
	ProjMovement->Velocity = direction * ProjMovement->InitialSpeed;
}



void AFloatingFireball::Reflect_Implementation(FVector direction, float force, int damage, AActor* initiator)
{
	ProjMovement->Velocity = direction * force;
	RefreshBounces();
	FString output = "Reflected: Damage=";
	output += FString::FromInt(damage);
	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, output);
}


void AFloatingFireball::Bounce(const FHitResult& r, const FVector& ImpactVelocity)
{
	Bounces += 1;
	if (Bounces >= MaxBounces)
	{
		this->Destroy();
	}
}

void AFloatingFireball::RefreshBounces()
{
	Bounces = 0;
}

/// <summary>
///		On Hit, Deal Damage to the Player or Enemy Hit and then disappear.
/// </summary>
void AFloatingFireball::OnHit(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AActor* PlayerCharacter = Cast<AHellOfARacketCharacter>(OtherActor);
	AActor* EnemyCharacter = Cast<AEnemy>(OtherActor);

	// Must be player or enemy who overlaps
	if (this != OtherActor && (OtherActor == PlayerCharacter || OtherActor == EnemyCharacter))
	{
		// Cause Damage and Get Destroyed chump
		FDamageEvent DamageEvent;

		//The Hit Actor will take Damage
		OtherActor->TakeDamage(DamageAmount, DamageEvent, nullptr, nullptr);

		Destroy();
	}
}

