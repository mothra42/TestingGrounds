// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ThrowableItem.generated.h"

UCLASS()
class TESTINGGROUNDS_API AThrowableItem : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AThrowableItem();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void LaunchItem(FVector StartLocation, FVector EndLocation);

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	class UProjectileMovementComponent* ProjectileMovement;

	//also has out parameter of velocity vector
	bool HasAimSolution(FVector StartLocation, FVector EndLocation, FVector& LaunchVelocity);

	float LaunchSpeed = 2000.f;
};
