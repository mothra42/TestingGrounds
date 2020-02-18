// Fill out your copyright notice in the Description page of Project Settings.


#include "HarpoonTrap.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include"../../../Engine/Plugins/Runtime/CableComponent/Source/CableComponent/Classes/CableComponent.h"
#include "Math/Vector.h"
#include "../Character/Mannequin.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "Components/PrimitiveComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "DrawDebugHelpers.h"

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

	if (IsTrapActivated)
	{
		ApplyForce();
	}
}

void AHarpoonTrap::OrientTrapToSurface(FHitResult HitResult)
{
	//TODO this is not exactly correct, but will do for now.
	FRotator NormalRotation = HitResult.Normal.GetSafeNormal().Rotation();
	FRotator TrapRotation = TrapBody->GetForwardVector().Rotation();
	FRotator NecessaryRotation = NormalRotation - TrapRotation;
	TrapBody->AddWorldRotation(NecessaryRotation);
}

void AHarpoonTrap::FireTrap(AMannequin* ActorToTrap)
{
	//Attach cable component to actor and pull them
	if (ActorToTrap != nullptr)
	{
		//check that actor is not the player
		if (!ActorToTrap->IsPlayerControlled())
		{
			IsTrapActivated = true;
			TrappedActor = ActorToTrap;
			
			auto AnchorComp = ActorToTrap->GetAttachmentPoint();
			DrawDebugSphere(GetWorld(), AnchorComp->GetComponentLocation(), 50.f, 30, FColor::Red, true);
			Harpoon->SetVisibility(true);
			Harpoon->SetAttachEndToComponent(AnchorComp);
		}

	}
}

void AHarpoonTrap::ApplyForce()
{
	if (TrappedActor != nullptr)
	{
		auto AnchorComp = TrappedActor->GetAttachmentPoint();
		FVector ForceDirection = FindForceDirection(AnchorComp->GetComponentLocation(), TrapBody->GetComponentLocation()).GetSafeNormal();

		auto MovementComponent = Cast<UCharacterMovementComponent>(TrappedActor->GetMovementComponent());
		if (MovementComponent != nullptr)
		{
			MovementComponent->AddForce(ForceDirection * ForceMagnitude);

			//MovementComponent->DisableMovement();
			TrappedActor->GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
			TrappedActor->GetMesh()->SetAllBodiesSimulatePhysics(true);
		}
	}
}

FVector AHarpoonTrap::FindForceDirection(FVector End, FVector Start)
{
	return Start - End;
}