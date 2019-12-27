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

	UE_LOG(LogTemp, Warning, TEXT("Hey you're firing a fucking shotgun now!"));

	//TODO Make shotgun fire multiple projectiles at strange angles, maybe mess with range.
}