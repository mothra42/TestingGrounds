// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Mannequin.generated.h"

UCLASS()
class TESTINGGROUNDS_API AMannequin : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMannequin();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void UnPossessed() override;

	UPROPERTY(EditDefaultsOnly, Category = "Setup")
	TSubclassOf<class AGun> GunBlueprint;

	//Fire Method
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void PullTrigger();

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void ReleaseTrigger();

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void PickupWeapon(AGun* NewWeapon);

	UFUNCTION(BlueprintCallable, Category = "Item")
	void ThrowItem(AActor* Item);

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void SwitchHeldWeapon(AGun* NewHeldWeapon);

	bool GetAimDirection(FVector& AimDirection) const;

	class USceneComponent* GetAttachmentPoint() const;

	class USkeletalMeshComponent* GetThirdPersonMeshComponent() const;

private:
	//Pawn Mesh, 1st person view
	UPROPERTY(VisibleDefaultsOnly, Category = "Mesh")
	USkeletalMeshComponent* FPMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FPCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory", meta = (AllowPrivateAccess = "true"))
	class UInventoryComponent* InventoryComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon", meta = (AllowPrivateAccess = "true"))
	AGun* HeldGun;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory", meta = (AllowPrivateAccess = "true"))
	AGun* StoredGun;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "ThrowableItem", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class AHarpoonTrap> ThrowableItemClass; //TODO need to make a generic throwable item class

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory", meta = (AllowPrivateAccess = "true"))
	USceneComponent* AttachmentPoint;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Setup", meta = (AllowPrivateAccess = "true"))
	class APlayerController* PlayerController;

	const int32 MaxGunsInInventory = 2;

	UPROPERTY(EditDefaultsOnly)
	float CrosshairXLocation = 0.5;

	UPROPERTY(EditDefaultsOnly)
	float CrosshairYLocation = 0.3333;

	void HoldWeapon(AGun* Weapon);

	void StoreWeapon(AGun* Weapon);
};
