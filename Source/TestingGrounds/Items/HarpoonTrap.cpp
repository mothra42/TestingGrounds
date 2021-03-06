// Fill out your copyright notice in the Description page of Project Settings.


#include "HarpoonTrap.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include"../../../Engine/Plugins/Runtime/CableComponent/Source/CableComponent/Classes/CableComponent.h"
#include "../Character/Mannequin.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "Components/PrimitiveComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/SceneComponent.h"
#include "DrawDebugHelpers.h"

// Sets default values
AHarpoonTrap::AHarpoonTrap()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	TrapBody = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TrapBody"));

	FireTriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("FireTriggerBox"));
	FireTriggerBox->SetupAttachment(TrapBody);
	FireTriggerBox->SetRelativeScale3D(FVector(500.f, 1.f, 1.f));
	FireTriggerBox->SetRelativeLocation(FVector(16000.f, 0.f, 0.f));

	TrapTriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TrapTriggerBox"));
	TrapTriggerBox->SetupAttachment(TrapBody);
	TrapTriggerBox->SetRelativeScale3D(FVector(14.25, 2.f, 1.75));
	TrapTriggerBox->SetRelativeLocation(FVector(300.f, 0.f, 0.f));

	CharacterAttachmentPoint = CreateDefaultSubobject<USceneComponent>(TEXT("CharacterAttachmentPoint"));
	CharacterAttachmentPoint->SetupAttachment(TrapBody);
	CharacterAttachmentPoint->SetRelativeLocation(FVector(100.f, 0.f, 0.f));

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
		//ApplyForce();
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
	if (ActorToTrap != nullptr && TrappedActor == nullptr)
	{
		//check that actor is not the player
		if (!ActorToTrap->IsPlayerControlled())
		{
			IsTrapActivated = true;
			TrappedActor = ActorToTrap;
			
			auto AnchorComp = ActorToTrap->GetAttachmentPoint();
			Harpoon->SetVisibility(true);
			Harpoon->SetAttachEndToComponent(AnchorComp);

			auto Skeleton = TrappedActor->GetThirdPersonMeshComponent();
			Skeleton->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
			Skeleton->SetAllBodiesSimulatePhysics(true);
		}
	}
}

void AHarpoonTrap::ApplyForce()
{
	//TODO this is still applied for some reason even after it shouldn't.
	UE_LOG(LogTemp, Warning, TEXT("Still applying force"));
	if (TrappedActor != nullptr)
	{
		auto AnchorComp = TrappedActor->GetAttachmentPoint();
		FVector ForceDirection = FindForceDirection(AnchorComp->GetComponentLocation(), TrapBody->GetComponentLocation()).GetSafeNormal();
		
		TrappedActor->GetThirdPersonMeshComponent()->AddForce(ForceDirection * ForceMagnitude);
	}
}

FVector AHarpoonTrap::FindForceDirection(FVector End, FVector Start)
{
	return Start - End;
}

void AHarpoonTrap::AttachActorToTrap()
{
	UE_LOG(LogTemp, Warning, TEXT("Actor is Attached"));
	if (IsTrapActivated)
	{
		TrappedActor->GetThirdPersonMeshComponent()->AttachToComponent(CharacterAttachmentPoint, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
		IsTrapActivated = false;
	}
}