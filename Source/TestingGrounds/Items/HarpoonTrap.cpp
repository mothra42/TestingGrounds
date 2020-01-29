// Fill out your copyright notice in the Description page of Project Settings.


#include "HarpoonTrap.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include"../../../Engine/Plugins/Runtime/CableComponent/Source/CableComponent/Classes/CableComponent.h"

// Sets default values
AHarpoonTrap::AHarpoonTrap()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	TrapBody = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TrapBody"));

	TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
	TriggerBox->SetupAttachment(TrapBody);

	Harpoon = CreateDefaultSubobject<UCableComponent>(TEXT("Harpoon"));
	Harpoon->SetupAttachment(TrapBody);
}

// Called when the game starts or when spawned
void AHarpoonTrap::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AHarpoonTrap::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

FHitResult AHarpoonTrap::AttachTrapToSurface()
{
	FHitResult HitResult;

	//bool HitAnchorPoint = GetWorld()->LineTraceSingleByChannel(
	//	HitResult,
	//	RayCastStart,
	//	RayCastEnd,
	//	ECollisionChannel::ECC_Visibility
	//);

	return HitResult;
}

void AHarpoonTrap::OrientTrapToSurface(FHitResult HitResult)
{
	//TODO Orient Trap to so that trap forward vector is normal to plane trap is attached to.
}

void AHarpoonTrap::FireTrap(AActor* ActorToTrap)
{
	//Attach cable component to actor and pull them
	if (ActorToTrap != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Actor that is trapped is %s"), *ActorToTrap->GetName());
	}
	//restrain trapped actor, maybe make them unconscious
}
