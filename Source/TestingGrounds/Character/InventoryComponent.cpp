// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryComponent.h"
#include "..//Weapons/Gun.h"

// Sets default values for this component's properties
UInventoryComponent::UInventoryComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

AActor* UInventoryComponent::SwitchWeapon(AActor* HeldWeapon)
{
	return nullptr;
}

void UInventoryComponent::AddWeapon(AActor* WeaponToBeAdded)
{
	AGun* GunToBeAdded = Cast<AGun>(WeaponToBeAdded);

	if (GunToBeAdded != nullptr)
	{
		WeaponInventory.Add(GunToBeAdded);
	}
}

void UInventoryComponent::RemoveWeapon(AActor* WeaponToBeRemoved)
{
	for (int32 i = 0; i < WeaponInventory.Num(); i++)
	{
		if (WeaponInventory[i] == WeaponToBeRemoved)
		{
			WeaponInventory.RemoveAt(i);
		}
	}
}

AActor* UInventoryComponent::EquipItem()
{
	return nullptr;
}

int32 UInventoryComponent::GetNumGunsInInventory()
{
	return WeaponInventory.Num();
}