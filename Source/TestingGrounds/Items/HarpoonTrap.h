// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ThrowableItem.h"
#include "HarpoonTrap.generated.h"

UCLASS()
class TESTINGGROUNDS_API AHarpoonTrap : public AThrowableItem
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
	class UBoxComponent* FireTriggerBox;

	UPROPERTY(Category = Trap, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UBoxComponent* TrapTriggerBox;

	UPROPERTY(Category = Trap, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class USceneComponent* CharacterAttachmentPoint;

	UPROPERTY(EditDefaultsOnly, Category = Trap)
	float ForceMagnitude = 2000000.0f;

	UFUNCTION(BlueprintCallable, Category = Attachment)
	void OrientTrapToSurface(FHitResult HitResult);

	UFUNCTION(BlueprintCallable, Category = Attachment)
	void AttachActorToTrap();

	bool IsTrapActivated = false;

	UPROPERTY(Category = Trap, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class AMannequin* TrappedActor;

	FVector FindForceDirection(FVector Start, FVector End);

	void ApplyForce();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "Firing")
	void FireTrap(AMannequin* ActorToTrap);
};
