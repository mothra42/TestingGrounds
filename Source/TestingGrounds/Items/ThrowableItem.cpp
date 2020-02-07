// Fill out your copyright notice in the Description page of Project Settings.


#include "ThrowableItem.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"

// Sets default values
AThrowableItem::AThrowableItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComp"));
	ProjectileMovement->bAutoActivate = false;
}

// Called when the game starts or when spawned
void AThrowableItem::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AThrowableItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AThrowableItem::LaunchItem(FVector StartLocation, FVector EndLocation)
{
	//TODO remove HasAimSolution method in favor of an aiming system.
	FVector LaunchVelocity;
	if (HasAimSolution(StartLocation, EndLocation, LaunchVelocity))
	{
		ProjectileMovement->SetVelocityInLocalSpace(FVector::ForwardVector * 2000);
		ProjectileMovement->Activate();
	}
}

bool AThrowableItem::HasAimSolution(FVector StartLocation, FVector EndLocation, FVector& LaunchVelocity)
{
	return UGameplayStatics::SuggestProjectileVelocity(
		this,
		LaunchVelocity,
		StartLocation,
		EndLocation,
		LaunchSpeed,
		false,
		0,
		0,
		ESuggestProjVelocityTraceOption::DoNotTrace
	);
}