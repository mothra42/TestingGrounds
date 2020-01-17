// Fill out your copyright notice in the Description page of Project Settings.


#include "GrapplingGun.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/Character.h"
#include "GameFramework/PawnMovementComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include"../../../Engine/Plugins/Runtime/CableComponent/Source/CableComponent/Classes/CableComponent.h"

AGrapplingGun::AGrapplingGun()
{
	PrimaryActorTick.bCanEverTick = true;

	CableComponent = CreateDefaultSubobject<UCableComponent>(TEXT("Cable Component"));
	Anchor = CreateDefaultSubobject<USceneComponent>(TEXT("Hook"));
}

void AGrapplingGun::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AGrapplingGun::OnFire()
{
	//Super::OnFire();

	//find out if end will attach to anything solid
	if (bHasAnchor(AnchorPoint))
	{
		UE_LOG(LogTemp, Warning, TEXT("Firing"));
		Anchor->SetWorldLocation(AnchorPoint);
		CableComponent->SetAttachEndToComponent(Anchor);
		CableComponent->SetVisibility(true);
	}
	//side note: can used the opposite to make a harpoon trap like thing.
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
	DrawDebugLine(GetWorld(), RayCastStart, RayCastEnd, FColor::Red, false, 5.f);
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
	//for some reason the cable component is in world space and this value stays fixed to the world, rather than relative location.
	//therefore this is a hack to fix it. TODO Make sure the cable component location is in relative space.
	CableComponent->SetWorldLocation(Super::FP_MuzzleLocation->GetComponentLocation());

	if (PlayerCharacter != nullptr)
	{
		auto Character = Cast<ACharacter>(PlayerCharacter);
		if (Character != nullptr)
		{
			auto MovementComponent = Cast<UCharacterMovementComponent>(Character->GetMovementComponent());
			if (MovementComponent != nullptr)
			{
				MovementComponent->AddForce(CalculateForce());
				MovementComponent->AddForce(PlayerCharacter->GetActorForwardVector() * 50000);
			}
		}
		UE_LOG(LogTemp, Warning, TEXT("Applying a force to character"));
		return;
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
