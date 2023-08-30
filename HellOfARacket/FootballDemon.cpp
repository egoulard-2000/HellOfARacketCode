// Fill out your copyright notice in the Description page of Project Settings.


#include "FootballDemon.h"
#include "HellOfARacketCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/PrimitiveComponent.h"


// Sets default values
AFootballDemon::AFootballDemon()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ChargeCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("ChargeCollider"));
	ChargeCollider->InitBoxExtent(FVector(50.0f, 55.0f, 100.0f));
	ChargeCollider->SetRelativeLocation(FVector(102.0f, 0.0f, 20.0f));
	ChargeCollider->SetGenerateOverlapEvents(true);
	//ChargeCollider->OnComponentBeginOverlap.AddDynamic(this, &AFootballDemon::OnOverlapBegin);

	ChargeCollider->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AFootballDemon::BeginPlay()
{
	Super::BeginPlay();
	EnableAttackCollision(false);
}

// Called every frame
void AFootballDemon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// For when the FootballDemon Lands on the Ground Upon First Contact
void AFootballDemon::Landed(const FHitResult& Hit)
{
	Super::Landed(Hit);

	#pragma region Deal Ground Pound Damage

	// Spawn Collider only when initiating 'Ground Pound' Motion 
	if (InGroundPoundState)
	{
		FCollisionShape SphereCollider = FCollisionShape::MakeSphere(GroundPoundImpactRadius);
		bool SweepHit = GetWorld()->SweepMultiByChannel(HitActors, Hit.Location, Hit.Location + FVector(0.01f, 0.01f, 0.01f), 
			FQuat::Identity, ECC_WorldStatic, SphereCollider);

		#pragma region Player Camera Shake

		AHellOfARacketCharacter* PlayerCharacter = Cast<AHellOfARacketCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
		if (PlayerCharacter == nullptr)
			return;
		if (!PlayerCharacter->GetCharacterMovement()->IsFalling())
			PlayerCharacter->ShakeCamera(FootballDemonGroundPoundShake);

		#pragma endregion Player Camera Shake

		/*DrawDebugSphere(GetWorld(), Hit.Location, GroundPoundImpactRadius, 50, FColor::Orange, false, 2.0f);*/

		if (SweepHit)
		{
			for (const auto& FoundHit : HitActors)
			{
				PlayerCharacter = Cast<AHellOfARacketCharacter>(FoundHit.GetActor());
				if (PlayerCharacter)
				{
					FDamageEvent DamageEvent;

					//The Hit Actor will take Damage (Cause Damage and Get Destroyed Chump)
					PlayerCharacter->TakeDamage(GroundPoundAttack_DamageAmount, DamageEvent, nullptr, nullptr);

					// Apply More/Less Knockback to player depending on how close they are to Demon
					FVector DistanceFromFootballDemon = GetActorLocation() - PlayerCharacter->GetActorLocation();
					const float KnockbackForce = FMath::Abs(GroundPoundAttack_KnockbackMaxDistance - DistanceFromFootballDemon.Length());
					PlayerCharacter->LaunchCharacter((FVector)KnockbackForce, true, true);

					break;
				}
			}
		}
	}
	InGroundPoundState = false;

	#pragma endregion Deal Ground Pound Damage
}

/// <summary>
///		Called by the Football AI Controller to dash at the Player
/// </summary>
void AFootballDemon::ChargeAttack()
{
	EnableAttackCollision(true);

	const FVector& ForwardDir = GetActorRotation().Vector();

	LaunchCharacter(ForwardDir * DashDistance, true, true);
}

/// <summary>
///		Called by the Football AI Controller to Jump at Player and Ground Pound Area to deal radial damage
/// </summary>
void AFootballDemon::GroundSmashAttack()
{
	// Player Location in Scene
	const ACharacter* PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	if (PlayerCharacter == nullptr)
		return;
	const FVector& PlayerLocation = PlayerCharacter->GetActorLocation();

	// FootballDemon in Scene
	const FVector& FootballDemonLocation = GetActorLocation();
	const FVector& ForwardDir = GetActorRotation().Vector();

	// Calculate Distance between Player and Football Demon
	const float Distance = FVector::Distance(FootballDemonLocation, PlayerLocation);

	// Start Jumping
	FVector Jump = ForwardDir * Distance;
	Jump.Z += JumpHeight;
	LaunchCharacter(Jump, true, true);

	// Let FootballDemon Jump for a bit, then activate ground pound motion after 'AirTime' seconds
	GetWorldTimerManager().SetTimer(AirTimeHandle, this, &AFootballDemon::EndAirTime, AirTime, false);
}

/// <summary>
///		Initiates when Air Time duration is met
/// </summary>
void AFootballDemon::EndAirTime()
{
	// Execute "Ground Pound" Motion
	InGroundPoundState = true;
	LaunchCharacter(FVector(0.0f, 0.0f, -1000.0f * GroundPoundSpeed), true, true);
}

/// <summary>
///		On Hit, Deal Damage to the Player
/// </summary>
void AFootballDemon::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ACharacter* PlayerCharacter = Cast<AHellOfARacketCharacter>(OtherActor);
	if (PlayerCharacter == nullptr)
		return;

	// UE_LOG(LogTemp, Warning, TEXT("Hit Player"));
	// Must be player who overlaps
	if (this != OtherActor && PlayerCharacter)
	{
		FDamageEvent DamageEvent;

		//The Hit Actor will take Damage (Get Destroyed Chump)
		PlayerCharacter->TakeDamage(ChargeAttack_DamageAmount, DamageEvent, nullptr, this);

		// Apply Knockback to player
		PlayerCharacter->LaunchCharacter((FVector)ChargeAttack_KnockbackDistance, true, true);

		EnableAttackCollision(false);
	}
}

/// <summary>
///		Turns charge attack collision On (true) or off (false)
/// </summary>
void AFootballDemon::EnableAttackCollision(const bool Enabled)
{
	if (Enabled)
		ChargeCollider->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	else
		ChargeCollider->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}
