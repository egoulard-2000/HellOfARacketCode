// Fill out your copyright notice in the Description page of Project Settings.


#include "Pickup.h"
#include "HellOfARacketCharacter.h"

// Sets default values
APickup::APickup()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Sphere Radius Collider Setup
	PickupRangeTrigger = CreateDefaultSubobject<USphereComponent>(TEXT("Pickup Range"));
	PickupRangeTrigger->SetSphereRadius(PickupRange);
	PickupRangeTrigger->SetGenerateOverlapEvents(true);
	PickupRangeTrigger->OnComponentBeginOverlap.AddDynamic(this, &APickup::OnOverlapBegin);

	RootComponent = PickupRangeTrigger;
}


// Handles moving the Item Spawned to the Player Location
void APickup::ItemInterp(float DeltaTime, float Value)
{
	// Current Health Kit in Scene
	StartLocation = GetActorLocation();

	// Must Travel to Player, therefore that's the end location for interpolation
	const ACharacter* PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	if (PlayerCharacter == nullptr)
		return;

	EndLocation = PlayerCharacter->GetActorLocation();

	const FVector& CurrentPlayerLocation = FMath::VInterpTo(StartLocation, EndLocation, DeltaTime, Value);
	SetActorLocation(CurrentPlayerLocation);
}

// Called when the game starts or when spawned
void APickup::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void APickup::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	AnimatePickup(DeltaTime);
	
	if (IsCollide)
		ItemInterp(DeltaTime, 20.0f);
}

// Handles Collision with the Player
void APickup::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	const AActor* PlayerActor = Cast<AHellOfARacketCharacter>(OtherActor);

	// Must be player who overlaps
	if (this != OtherActor && OtherActor == PlayerActor)
	{
		// Interp To Player and Add Health
		IsCollide = true;
		GetWorldTimerManager().SetTimer(InterpTimer, this, &APickup::ActivatePickup, 0.07f, false);
	}
}

void APickup::AnimatePickup(float DeltaTime)
{
	FVector NewLocation = GetActorLocation();
	float DeltaHeight = (FMath::Sin(RunningTime + DeltaTime) - FMath::Sin(RunningTime));
	NewLocation.X += DeltaHeight * XValue;
	NewLocation.Y += DeltaHeight * YValue;
	NewLocation.Z += DeltaHeight * ZValue;
	RunningTime += DeltaTime;
	SetActorLocation(NewLocation);

	FQuat QuatRotation = FQuat(FRotator(PitchValue, YawValue, RollValue));
	AddActorLocalRotation(QuatRotation, false, 0, ETeleportType::None);
}

// Self-Explanatory
void APickup::ActivatePickup() {}