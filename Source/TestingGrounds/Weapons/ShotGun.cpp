// Fill out your copyright notice in the Description page of Project Settings.


#include "ShotGun.h"

void AShotGun::OnFire()
{
	Super::OnFire();

	UE_LOG(LogTemp, Warning, TEXT("Hey you're firing a fucking shotgun now!"));
}