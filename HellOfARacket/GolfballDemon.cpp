// Fill out your copyright notice in the Description page of Project Settings.


#include "GolfballDemon.h"
#include "Kismet/GameplayStatics.h"
#include "Projectile.h"
#include "Kismet/KismetMathLibrary.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BrainComponent.h"


// Sets default values
AGolfballDemon::AGolfballDemon()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	ProjectileSpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("SpawnPoint"));
	ProjectileSpawnPoint->SetupAttachment(RootComponent);

}

// Called when the game starts or when spawned
void AGolfballDemon::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AGolfballDemon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AGolfballDemon::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

/// <summary>
///		Function responsible for shooting the player with projectile
/// </summary>
void AGolfballDemon::ShootAttack()
{
	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);

	if(PlayerPawn)
	{
	 	RotateProjectileSpawnPointTowardsPlayer(PlayerPawn->GetActorLocation());

	 	AProjectile *spawnedProjectile = 
	 	GetWorld()->SpawnActor<AProjectile>(
	 		projectileClass, 
	 		ProjectileSpawnPoint->GetComponentLocation(), 
	 		ProjectileSpawnPoint->GetComponentRotation() );

		IReflectable::Execute_SetReflectOwner(spawnedProjectile, this);
		UProjectileMovementComponent* ProjMovement = spawnedProjectile->FindComponentByClass<UProjectileMovementComponent>();
		FVector PlayerVelocity = PlayerPawn->GetVelocity();
		PlayerVelocity.X = 0;
		FVector ProjectileVelocity = GetProjectileStartVelocity(spawnedProjectile->GetActorLocation(), PlayerPawn->GetActorLocation(), PlayerVelocity);
		
		ProjMovement->Velocity = ProjectileVelocity;
	}
}

void AGolfballDemon::RotateProjectileSpawnPointTowardsPlayer(FVector PlayerLocation)
{
	FVector PointToTarget = PlayerLocation - ProjectileSpawnPoint->GetComponentLocation();
    FRotator LookAtRotation = FRotator(PointToTarget.Rotation().Pitch, PointToTarget.Rotation().Yaw, 0.f);
    ProjectileSpawnPoint->SetWorldRotation(LookAtRotation);
}







