// Fill out your copyright notice in the Description page of Project Settings.


#include "GrapplingGun.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/Character.h"
#include "GameFramework/PawnMovementComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

void AGrapplingGun::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AGrapplingGun::OnFire()
{
	//Super::OnFire();

	//find out if end will attach to anything solid
	if (bHasAnchor(HitLocation))
	{
		//launch cable component that anchors to the hit point.
	}
	//side note: can used the opposite to make a harpoon trap like thing.
}

bool AGrapplingGun::bHasAnchor(FVector& HitLocation)
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
		HitLocation = HitResult.Location;
		UE_LOG(LogTemp, Warning, TEXT("Anchor found, Location is [%s]"), *HitLocation.ToString());
		DrawDebugLine(GetWorld(), RayCastStart, RayCastEnd, FColor::Red, false, 5.f);
		isAttached = true;
		return true;
	}

	UE_LOG(LogTemp, Warning, TEXT("No Anchor found"));
	HitLocation = FVector(0);
	isAttached = false;
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
				MovementComponent->AddForce(PlayerCharacter->GetActorForwardVector() * 50000);
			}
		}
		UE_LOG(LogTemp, Warning, TEXT("Applying a force to character"));
		return;
	}

	return;
}

FVector AGrapplingGun::CalculateForce()
{
	FVector PlayerVelocity = PlayerCharacter->GetVelocity();
	FVector DistanceVector = PlayerCharacter->GetActorLocation() - HitLocation;
	FVector NormalizedDistanceVector = DistanceVector.GetSafeNormal();

	FVector GrappleForce = (FVector::DotProduct(PlayerVelocity, DistanceVector) * NormalizedDistanceVector) * -2.0;
	return GrappleForce;
}
