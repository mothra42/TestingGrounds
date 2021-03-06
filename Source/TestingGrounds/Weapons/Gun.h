// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Gun.generated.h"

class UInputComponent;

UCLASS()
class TESTINGGROUNDS_API AGun : public AActor
{
	GENERATED_BODY()

	/** Gun mesh*/
	UPROPERTY(VisibleDefaultsOnly, Category = "Mesh")
	class USkeletalMeshComponent* FP_Gun;

	UPROPERTY(VisibleDefaultsOnly, Category = "Movement")
	class URotatingMovementComponent* RotationComponent;

	UPROPERTY(VisibleDefaultsOnly, Category = "Collision")
	class UCapsuleComponent* FP_GunCapsule;

protected:

	/** Location on gun mesh where projectiles should spawn. */
	UPROPERTY(VisibleDefaultsOnly, Category = "Mesh")
	class USceneComponent* FP_MuzzleLocation;

	virtual void BeginPlay() override;

public:	
	// Sets default values for this actor's properties
	AGun();

	//virtual void Tick(float DeltaTime) override;

	/** Projectile class to spawn */
	UPROPERTY(EditDefaultsOnly, Category = "Projectile")
	TSubclassOf<class ABallProjectile> ProjectileClass;

	/** Sound to play each time we fire */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay")
	class USoundBase* FireSound;

	/** AnimMontage to play each time we fire */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay")
	class UAnimMontage* FPFireAnimation;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay")
	UAnimMontage* TPFireAnimation;

	class UAnimInstance* FPAnimInstance;

	UAnimInstance* TPAnimInstance;

	UFUNCTION(BlueprintCallable, Category = "Firing")
	virtual void OnFire(FVector AimDirection);

	UFUNCTION(BlueprintCallable, Category = "Firing")
	virtual void OnReleaseTrigger();

	void DeactivateRotationComponent();

	void DeactivateCapsuleComponent();
};
