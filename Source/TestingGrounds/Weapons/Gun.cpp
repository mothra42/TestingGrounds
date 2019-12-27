// Fill out your copyright notice in the Description page of Project Settings.


#include "Gun.h"
#include "BallProjectile.h"
#include "Kismet/GameplayStatics.h"
#include "Animation/AnimInstance.h"
#include "Components/InputComponent.h"
#include "GameFramework/InputSettings.h"
#include "GameFramework/RotatingMovementComponent.h"
#include "Components/CapsuleComponent.h"

// Sets default values
AGun::AGun()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create a gun mesh component
	FP_Gun = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FP_Gun"));
	FP_Gun->bCastDynamicShadow = false;
	FP_Gun->CastShadow = false;

	FP_MuzzleLocation = CreateDefaultSubobject<USceneComponent>(TEXT("MuzzleLocation"));
	FP_MuzzleLocation->SetupAttachment(FP_Gun);
	FP_MuzzleLocation->SetRelativeLocation(FVector(0.2f, 48.4f, -10.6f));

	FP_GunCapsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Collision Capsule"));
	FP_GunCapsule->SetupAttachment(FP_Gun);
	FP_GunCapsule->SetRelativeScale3D(FVector(2.325f, 2.265f, 1.1025f));


	RotationComponent = CreateDefaultSubobject<URotatingMovementComponent>(TEXT("RotatingMovementComponent"));
	RotationComponent->bRotationInLocalSpace = false;

}

void AGun::BeginPlay()
{
	Super::BeginPlay();
}

void AGun::OnFire()
{
	// try and fire a projectile
	if (ProjectileClass != NULL)
	{
		UWorld* const World = GetWorld();
		if (World != NULL)
		{
			const FRotator SpawnRotation = FP_MuzzleLocation->GetComponentRotation();
			// MuzzleOffset is in camera space, so transform it to world space before offsetting from the character location to find the final muzzle position
			const FVector SpawnLocation = FP_MuzzleLocation->GetComponentLocation();

			UWorld* const World = GetWorld();
			if (World != NULL)
			{
				// spawn the projectile at the muzzle
				World->SpawnActor<ABallProjectile>(ProjectileClass, SpawnLocation, SpawnRotation);
			}
		}
	}

	// try and play the sound if specified
	if (FireSound != nullptr)
	{
		UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
	}

	//try and play a firing animation if specified
	if (FPFireAnimation != nullptr && FPAnimInstance != nullptr)
	{
		FPAnimInstance->Montage_Play(FPFireAnimation, 1.f);
	}

	if (TPFireAnimation != nullptr && TPAnimInstance != nullptr)
	{
		TPAnimInstance->Montage_Play(TPFireAnimation, 1.f);
	}
	return;
}

void AGun::DeactivateRotationComponent()
{
	RotationComponent->Deactivate();
}

void AGun::DeactivateCapsuleComponent()
{
	FP_GunCapsule->Deactivate();
}