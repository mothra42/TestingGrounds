// Fill out your copyright notice in the Description page of Project Settings.


#include "GrapplingGun.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/Character.h"
#include "GameFramework/PawnMovementComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "DrawDebugHelpers.h"
#include"../../../Engine/Plugins/Runtime/CableComponent/Source/CableComponent/Classes/CableComponent.h"

AGrapplingGun::AGrapplingGun()
{
	PrimaryActorTick.bCanEverTick = true;

	CableComponent = CreateDefaultSubobject<UCableComponent>(TEXT("Cable Component"));
	CableComponent->SetupAttachment(Super::FP_MuzzleLocation);
	Anchor = CreateDefaultSubobject<USceneComponent>(TEXT("Hook"));
}

void AGrapplingGun::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsRetractionForceBeingApplied)
	{
		ApplyRetractionForce();
	}
}

void AGrapplingGun::OnFire(FVector AimDirection)
{
	//find out if end will attach to anything solid
	if (bHasAnchor(AnchorPoint))
	{
		Anchor->SetWorldLocation(AnchorPoint);
		CableComponent->SetAttachEndToComponent(Anchor);
		CableComponent->SetVisibility(true);
		float Distance = (PlayerCharacter->GetActorLocation() - AnchorPoint).Size();
		CableComponent->CableLength = Distance;
	}
}

void AGrapplingGun::OnReleaseTrigger()
{
	IsAttached = false;
	CableComponent->SetVisibility(false);
}

bool AGrapplingGun::bHasAnchor(FVector& AnchorPoint)
{
	FHitResult HitResult;
	
	auto GunMuzzleDirection = Super::FP_MuzzleLocation->GetForwardVector();
	auto RayCastStart = Super::FP_MuzzleLocation->GetComponentLocation();
	auto RayCastEnd = RayCastStart + (GunMuzzleDirection * CableRange);

	bool HitAnchorPoint = GetWorld()->LineTraceSingleByChannel(
		HitResult,
		RayCastStart,
		RayCastEnd,
		ECollisionChannel::ECC_Visibility
	);
	if (HitAnchorPoint)
	{
		AnchorPoint = HitResult.Location;
		IsAttached = true;
		return true;
	}

	AnchorPoint = FVector(0);
	IsAttached = false;
	return false;
}

void AGrapplingGun::ApplyForceToCharacter()
{
	if (PlayerCharacter != nullptr)
	{
		auto Character = Cast<ACharacter>(PlayerCharacter);
		if (Character != nullptr)
		{
			auto MovementComponent = Cast<UCharacterMovementComponent>(Character->GetMovementComponent());
			if (MovementComponent != nullptr)
			{
				MovementComponent->AddForce(CalculateForce());
				MovementComponent->AddForce(PlayerCharacter->GetActorForwardVector() * 50000); //TODO make configurable in BP
				float CableLength = CableComponent->CableLength;
			}
		}
	}
}

FVector AGrapplingGun::CalculateForce()
{
	FVector PlayerVelocity = PlayerCharacter->GetVelocity();
	FVector DistanceVector = PlayerCharacter->GetActorLocation() - AnchorPoint;
	FVector NormalizedDistanceVector = DistanceVector.GetSafeNormal();
	FVector GrappleForce = (FVector::DotProduct(PlayerVelocity, DistanceVector) * NormalizedDistanceVector) * -2.0;
	return GrappleForce;
}

void AGrapplingGun::ApplyRetractionForce()
{
	if (PlayerCharacter != nullptr)
	{
		FVector PlayerLocation = PlayerCharacter->GetActorLocation();

		FVector ForceDirection = (AnchorPoint - PlayerLocation).GetSafeNormal();

		auto Character = Cast<ACharacter>(PlayerCharacter);
		if (Character != nullptr)
		{
			auto MovementComponent = Cast<UCharacterMovementComponent>(Character->GetMovementComponent());
			if (MovementComponent != nullptr)
			{
				MovementComponent->AddForce(ForceDirection * 5000000); //TODO make configurable in BP
				float CableLength = CableComponent->CableLength;
			}
		}
	}	
}