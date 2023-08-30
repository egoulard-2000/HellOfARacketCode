// Fill out your copyright notice in the Description page of Project Settings.


#include "SoccerDemon.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/PrimitiveComponent.h"
#include "HellOfARacketCharacter.h"

ASoccerDemon::ASoccerDemon()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ProjectileSpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("Projectile Spawn Point"));
	ProjectileSpawnPoint->SetupAttachment(RootComponent);

	ChaseCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("Chase Collider"));
	ChaseCollider->InitBoxExtent(FVector(200.0f, 200.0f, 200.0f));
	ChaseCollider->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
	ChaseCollider->SetGenerateOverlapEvents(true);
	ChaseCollider->OnComponentBeginOverlap.AddDynamic(this, &ASoccerDemon::OnOverlapBegin);

	ChaseCollider->SetupAttachment(RootComponent);
}

void ASoccerDemon::BeginPlay()
{
	Super::BeginPlay();
	EnableChaseAttackCollision(false);
}

void ASoccerDemon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ASoccerDemon::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, 
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AHellOfARacketCharacter* PlayerCharacter = Cast<AHellOfARacketCharacter>(OtherActor);
	if (PlayerCharacter != nullptr)
	{
		// Deal Damage To Player
		FDamageEvent DamageEvent;
		PlayerCharacter->TakeDamage(ChaseAttack_DamageAmount, DamageEvent, nullptr, this);

		// Apply Player Knockback
		PlayerCharacter->LaunchCharacter((FVector)ChaseAttack_KnockbackDistance, true, true);
	}
}

/// <summary>
///		Function responsible for shooting the player with projectile
/// </summary>
void ASoccerDemon::ShootAttack()
{
	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);

	if (PlayerPawn)
	{
		AProjectile* SpawnedProjectile =
			GetWorld()->SpawnActor<AProjectile>(
				ProjectileType,
				ProjectileSpawnPoint->GetComponentLocation(),
				ProjectileSpawnPoint->GetComponentRotation());

		IReflectable::Execute_SetReflectOwner(SpawnedProjectile, this);
		UProjectileMovementComponent* ProjMovement = SpawnedProjectile->FindComponentByClass<UProjectileMovementComponent>();
		FVector PlayerVelocity = PlayerPawn->GetVelocity();
		PlayerVelocity.X = 0;
		FVector ProjectileVelocity = GetProjectileStartVelocity(SpawnedProjectile->GetActorLocation(), PlayerPawn->GetActorLocation(), PlayerVelocity);

		ProjMovement->Velocity = ProjectileVelocity;
	}
}

/// <summary>
///		Self-Explanatory
/// </summary>
/// <param name="Enabled"></param>
void ASoccerDemon::EnableChaseAttackCollision(const bool Enabled)
{
	if (Enabled)
		ChaseCollider->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	else
		ChaseCollider->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}
