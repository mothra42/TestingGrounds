// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapons/Gun.h"
#include "GrapplingGun.generated.h"

/**
 * 
 */
UCLASS()
class TESTINGGROUNDS_API AGrapplingGun : public AGun
{
	GENERATED_BODY()
	
	public:

		bool IsRetractionForceBeingApplied = false;

		AGrapplingGun();

		virtual void Tick(float DeltaTime) override;

		virtual void OnFire(FVector AimDirection) override;

		virtual void OnReleaseTrigger() override;

		void ApplyRetractionForce();

	private:
		UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Cable", meta = (AllowPrivateAccess = "true"))
		class UCableComponent* CableComponent;

		UPROPERTY(VisibleDefaultsOnly, Category = "Cable")
		class USceneComponent* Anchor;

		UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Cable", meta = (AllowPrivateAccess = "true"))
		FVector AnchorPoint;

		UPROPERTY(EditDefaultsOnly, Category = "Cable")
		float CableRange = 1000.f;

		UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Setup", meta = (AllowPrivateAccess = "true"))
		class AActor* PlayerCharacter;

		bool bHasAnchor(FVector& Hitlocation); // Out Param HitLocation, where target will attach;

		UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Grappling Hook", meta = (AllowPrivateAccess = "true"))
		bool IsAttached = false;

		UFUNCTION(BlueprintCallable, Category = "Grappling Hook")
		void ApplyForceToCharacter();

		FVector CalculateForce();
};
