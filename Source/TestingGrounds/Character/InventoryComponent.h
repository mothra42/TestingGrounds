// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InventoryComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TESTINGGROUNDS_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UInventoryComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	AActor* SwitchWeapon(AActor* HeldWeapon); //used for player to switch between weapons

	AActor* EquipItem();

private:
	AActor* WeaponOne;
	AActor* WeaponTwo;
	
	TArray<AActor*> ItemInventory;

};
