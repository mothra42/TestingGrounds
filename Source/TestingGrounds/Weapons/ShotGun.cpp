// Fill out your copyright notice in the Description page of Project Settings.


#include "ShotGun.h"
#include "BallProjectile.h"
#include "Kismet/GameplayStatics.h"
#include "Animation/AnimInstance.h"
#include "Components/InputComponent.h"
#include "GameFramework/InputSettings.h"

void AShotGun::OnFire()
{
	Super::OnFire();

	if (Super::ProjectileClass != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Hey you're firing a fucking shotgun now!"));
		UWorld* const World = GetWorld();

		if (World != nullptr)
		{
			const FRotator SpawnRotation = Super::FP_MuzzleLocation->GetComponentRotation();
			const FVector SpawnLocation = Super::FP_MuzzleLocation->GetComponentLocation();

			for (size_t i = 0; i < NumProjectiles; i++)
			{
				float RandPitch = FMath::RandRange(-ProjectileSpreadRange, ProjectileSpreadRange);
				float RandYaw = FMath::RandRange(-ProjectileSpreadRange, ProjectileSpreadRange);

				FRotator NewRotation = SpawnRotation + FRotator(RandPitch, RandYaw, 0.f);

				World->SpawnActor<ABallProjectile>(Super::ProjectileClass, SpawnLocation, NewRotation);
			}
		}
	}
}