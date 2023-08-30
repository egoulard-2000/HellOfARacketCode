// Fill out your copyright notice in the Description page of Project Settings.


#include "BossReflector.h"

// Sets default values for this component's properties
UBossReflector::UBossReflector()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	reflectRadius = 100;
	maxNumReflect = 3;
	reflectSpeed = 50;
	damage = 50000;

	timesReflected = 0;
	mostRecentProj = nullptr;
	parentActor = GetOwner();
}


// Called when the game starts
void UBossReflector::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UBossReflector::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

AProjectile* UBossReflector::getMostRecentProj()
{
	return mostRecentProj;
}

bool UBossReflector::isProjectile(AActor* actor)
{
	return UKismetSystemLibrary::DoesImplementInterface(actor, UReflectable::StaticClass());
}

bool UBossReflector::isReflectable(AActor* actor)
{
	AActor* player = UGameplayStatics::GetPlayerPawn(GEngine->GetWorld(), 0);
	AProjectile* proj;
	if (isProjectile(actor)) {
		proj = Cast<AProjectile>(actor);
	}
	else {
		return false;
	}
	return isProjectile(actor) && (player == proj->GetOwner());
}

void UBossReflector::onOverlap(AActor* actor)
{
	if (isReflectable(actor)) {
		AProjectile* proj = Cast<AProjectile>(actor);
		if (proj == mostRecentProj) {
			timesReflected++;
			if (timesReflected == maxNumReflect) {
				// takeDamage
				// destroyProjectile
			}
			else {
				proj->SetOwner(parentActor);
				reflectTowardPlayer(proj);
			}
		}
		else {
			timesReflected = 1;
			proj->SetOwner(parentActor);
			reflectTowardPlayer(proj);
		}
	}
}

void UBossReflector::reflectTowardPlayer(AProjectile* proj)
{
	AActor* player = UGameplayStatics::GetPlayerPawn(GEngine->GetWorld(), 0);
	FVector playerLoc = player->GetActorLocation();
	FVector projLoc = proj->GetActorLocation();

	FVector direction = playerLoc - projLoc;
	direction.Normalize();
	
	proj->Reflect_Implementation(direction, reflectSpeed, damage, this->GetOwner());
}

