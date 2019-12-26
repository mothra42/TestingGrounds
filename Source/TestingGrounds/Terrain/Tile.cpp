// Fill out your copyright notice in the Description page of Project Settings.


#include "Tile.h"
#include "Public/WorldCollision.h"
#include "DrawDebugHelpers.h"

// Sets default values
ATile::ATile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void ATile::PlaceActors(TSubclassOf<AActor> ToSpawn, int MinSpawn, int MaxSpawn, float Radius, bool ShouldScale, float MinScale, float MaxScale)
{
	int NumberToSpawn = FMath::RandRange(MinSpawn, MaxSpawn);
	
	for (size_t i = 0; i < NumberToSpawn; i++)
	{
		FVector SpawnLocation;
		FVector Scale = FVector(1,1,1);

		float RandScale = FMath::RandRange(MinScale, MaxScale);
		if (FindEmptyLocation(SpawnLocation, Radius * RandScale))
		{
			if (ShouldScale)
			{
				Scale = Scale * RandScale;
			}
			float RandRotation = FMath::RandRange(-180.f, 180.f);
			PlaceActor(ToSpawn, SpawnLocation, RandRotation, Scale);
		}
	}
}

bool ATile::FindEmptyLocation(FVector& OutLocation, float Radius)
{

	FVector Min(0, -2000, 0);
	FVector Max(4000, 2000, 0);
	FBox Bounds(Min, Max);
	const int32 MAX_ATTEMPTS = 25;

	for (size_t i = 0; i < MAX_ATTEMPTS; i++)
	{
		FVector RandLocation = FMath::RandPointInBox(Bounds);
		if (bCanSpawnAtLocation(RandLocation, Radius))
		{
			OutLocation = RandLocation;
			return true;
		}
	}

	return false;
}

void ATile::PlaceActor(TSubclassOf<AActor> ToSpawn, FVector SpawnPoint, float Rotation, FVector Scale)
{
	AActor* Spawned = GetWorld()->SpawnActor<AActor>(ToSpawn);
	Spawned->SetActorRelativeLocation(SpawnPoint);
	Spawned->AttachToActor(this, FAttachmentTransformRules(EAttachmentRule::KeepRelative, false));
	Spawned->SetActorRotation(FRotator(0, Rotation, 0));
	Spawned->SetActorScale3D(Scale);
}

// Called when the game starts or when spawned
void ATile::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ATile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

bool ATile::bCanSpawnAtLocation(FVector Location, float Radius)
{
	FHitResult HitResult;
	FVector GlobalLocation = ActorToWorld().TransformPosition(Location);

	bool HasHit = GetWorld()->SweepSingleByChannel(
		HitResult,
		GlobalLocation,
		GlobalLocation + FVector(1, 1, 1), //need to add a small amount for the sweep to work
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel2,
		FCollisionShape::MakeSphere(Radius)
	);
	
	//FColor ResultColor = HasHit ? FColor::Red : FColor::Green;
	//DrawDebugCapsule(GetWorld(), GlobalLocation, 0, Radius, FQuat::Identity, ResultColor, true, 100);

	return !HasHit;
}

