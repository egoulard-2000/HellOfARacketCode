// Fill out your copyright notice in the Description page of Project Settings.


#include "PickleBallSatan.h"
#include "Kismet/GameplayStatics.h"
#include "Projectile.h"
#include "FloatingFireball.h"
#include "Components/SphereComponent.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "HellOfARacketCharacter.h"

APickleBallSatan::APickleBallSatan()
{
	PrimaryActorTick.bCanEverTick = true;
	ProjectileSpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("SpawnPoint"));
	ProjectileSpawnPoint->SetupAttachment(RootComponent);

	FireballSpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("FireballSpawnPoint"));
	FireballSpawnPoint->SetupAttachment(RootComponent);

	satanSwingForce = 500.0f;
	MaxFireBreaths = 1;
}

void APickleBallSatan::BeginPlay()
{
	Super::BeginPlay();
	MyController = Cast<AAIController>(this->GetController());
	SatanBlackboard = MyController->GetBlackboardComponent();
	
}

void APickleBallSatan::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APickleBallSatan::Serve()
{
	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(this, 0);

	if (PlayerPawn)
	{
		// Rotates fire point towards player
		RotateProjectileSpawnPointTowardsPlayer(PlayerPawn->GetActorLocation());

		// Spawns the projectile actor
		AProjectile* spawnedProjectile =
			GetWorld()->SpawnActor<AProjectile>(
				projectileClass,
				ProjectileSpawnPoint->GetComponentLocation(),
				ProjectileSpawnPoint->GetComponentRotation());
				

		IReflectable::Execute_SetReflectOwner(spawnedProjectile, this);
		AActor* currActor = IReflectable::Execute_GetReflectOwner(spawnedProjectile); 
		//UE_LOG(LogTemp, Warning, TEXT("This is the data: %s"), *FString(currActor->GetName()));

		// Signal to Boss that projectile was destroyed?
		if (spawnedProjectile) {
			spawnedProjectile->OnProjectileDestroyed.BindUObject(this, &APickleBallSatan::ProjDestroyedEventFunction);
		}
	}

	RallyCounter++;
	// Set BB_RallyCounter to False so wno more projectiles are served until this projectile dies
	SatanBlackboard->SetValueAsBool("BB_RallyCounter", false);

	

}

/*
Bus Attack?
*/
void APickleBallSatan::ShootAttack()
{
	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(this, 0);

	if (PlayerPawn)
	{
		RotateProjectileSpawnPointTowardsPlayer(PlayerPawn->GetActorLocation());

		AProjectile* spawnedProjectile =
			GetWorld()->SpawnActor<AProjectile>(
				NULL,
				ProjectileSpawnPoint->GetComponentLocation(),
				ProjectileSpawnPoint->GetComponentRotation());

		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("Attacking from inside pickleball satan"));
	}

	/*FVector fireballPosition = FVector(0.0, 0.0, 0.0);
	FRotator fireballRotation = FRotator(0.0, 0.0, 0.0);*/
	AActor* spawnedFireball =
		GetWorld()->SpawnActor<AActor>(
			fireball,
			FireballSpawnPoint->GetComponentLocation(),
			FireballSpawnPoint->GetComponentRotation());


	/*FVector launcherPosition = FVector(0.0, 0.0, 0.0);
	FRotator launcherRotation = FRotator(0.0, 0.0, 0.0);*/
	AActor* spawnedLauncher =
		GetWorld()->SpawnActor<AActor>(
			launcher,
			ProjectileSpawnPoint->GetComponentLocation(),
			ProjectileSpawnPoint->GetComponentRotation());
}

void APickleBallSatan::RotateProjectileSpawnPointTowardsPlayer(FVector PlayerLocation)
{
	FVector PointToTarget = PlayerLocation - ProjectileSpawnPoint->GetComponentLocation();
	FRotator LookAtRotation = FRotator(PointToTarget.Rotation().Pitch, PointToTarget.Rotation().Yaw, 0.f);
	ProjectileSpawnPoint->SetWorldRotation(LookAtRotation);

	/*PointToTarget = PlayerLocation - FireballSpawnPoint->GetComponentLocation();
	LookAtRotation = FRotator(PointToTarget.Rotation().Pitch, PointToTarget.Rotation().Yaw, 0.f);*/
	FireballSpawnPoint->SetWorldRotation(LookAtRotation);
}

/*ds
Reflect Incoming Projectile
*/
void APickleBallSatan::Reflect(AProjectile* proj)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, FString::Printf(TEXT("Player Hits: %f"), RallyCounter));

	// If stage already over OR projectile ownder by satan OR RallyCounter went past max, do not reflect
    if (RallysWonByPlayer >= RallysNeededToProgress || IReflectable::Execute_GetReflectOwner(proj) == this || RallyCounter > MaxRallyCounter){
		return;
	}
	// If rally counter at 3, lose the rally
	if (RallyCounter == MaxRallyCounter)
	{
		LoseRally();
		RallyCounter++;
		return;
	}
	

	// Else Reflect
	IReflectable::Execute_SetReflectOwner(proj, this);
	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(this, 0);
	if (PlayerPawn)
	{
		// Rotates fire point towards player
		RotateProjectileSpawnPointTowardsPlayer(PlayerPawn->GetActorLocation());
		
	}
	FVector dir = PlayerPawn->GetActorLocation() - proj->GetActorLocation();
	proj->RefreshLifespan();

	dir.Normalize();
	IReflectable::Execute_Reflect(proj, dir, satanSwingForce, 10, this);

	RallyCounter++; 

	return;
}

void APickleBallSatan::LoseRally()
{
	// Turn Satan Reflection Back On After rallyLossCooldown seconds
	GetWorldTimerManager().SetTimer(ResetReflectHandle, this, &APickleBallSatan::ResetReflect, rallyLossCooldown, false);
	
	RallysWonByPlayer++;
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, FString::Printf(TEXT("Rallys Won: %f"), RallysWonByPlayer));

	// If player has won enough rallys, start next stage
	if (RallysWonByPlayer >= RallysNeededToProgress){
		SatanBlackboard->SetValueAsBool("ServeStageFinished", true);
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("ADVANCING TO NEXT STAGE"));
	}
	// Else let boss serve again
	else{
		SatanBlackboard->SetValueAsBool("BB_RallyCounter", true);
	}
	
	return;
}

void APickleBallSatan::ResetReflect(){
	RallyCounter = 0;
	GetWorldTimerManager().ClearTimer(ResetReflectHandle);
}


void APickleBallSatan::ProjDestroyedEventFunction() {
	RallyCounter = 0;
	SatanBlackboard->SetValueAsBool("BB_RallyCounter", true);
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT("The projectile was destroyed. Serving again"));
}

void APickleBallSatan::ResetToServeState() 
{
	SatanBlackboard->SetValueAsBool("ServeStageFinished", false);
	SatanBlackboard->SetValueAsBool("BB_RallyCounter", true);
	RallyCounter = 0;
	RallysWonByPlayer = 0;
	currStage = ServeStage;
	ResetReflect();
}

void APickleBallSatan::ResetFireAttack()
{
	SatanBlackboard->SetValueAsBool("CanFireBreath", false);
	FireBreaths = 0;
}



void APickleBallSatan::UpdateTotalFireBreaths()
{
	// We want to keep track of the total fire breaths made
	
	if (FireBreaths >= MaxFireBreaths)
	{
		SatanBlackboard->SetValueAsBool("CanFireBreath", true);
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, FString::Printf(TEXT("I can't fire breath again!")));

	}
	else {
		FireBreaths++;
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, FString::Printf(TEXT("Fire breaths so far: %f"), FireBreaths));
	}
}

