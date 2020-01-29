// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HarpoonTrap.generated.h"

UCLASS()
class TESTINGGROUNDS_API AHarpoonTrap : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AHarpoonTrap();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	UPROPERTY(Category = Trap, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* TrapBody;

	UPROPERTY(Category = Trap, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UCableComponent* Harpoon;

	UPROPERTY(Category = Trap, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* TriggerBox;

	void OrientTrapToSurface(FHitResult HitResult);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "Trap Placement")
	FHitResult AttachTrapToSurface();

	UFUNCTION(BlueprintCallable, Category = "Firing")
	void FireTrap(AActor* ActorToTrap);
};
