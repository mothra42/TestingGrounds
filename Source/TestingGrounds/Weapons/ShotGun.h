// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapons/Gun.h"
#include "ShotGun.generated.h"

/**
 * 
 */
UCLASS()
class TESTINGGROUNDS_API AShotGun : public AGun
{
	GENERATED_BODY()

public:
	virtual void OnFire() override;
};
