// Fill out your copyright notice in the Description page of Project Settings.


#include "Mannequin.h"
#include "Components/CapsuleComponent.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/SkeletalMeshSocket.h"
#include "InventoryComponent.h"
#include "GameFramework/PlayerController.h"
#include "../TestingGroundsHUD.h"
#include "Weapons/Gun.h"
#include "Items/HarpoonTrap.h"
#include "Components/SceneComponent.h"
#include "DrawDebugHelpers.h"

// Sets default values
AMannequin::AMannequin()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create First Person Camera
	FPCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FPCamera"));
	FPCamera->SetupAttachment(GetCapsuleComponent());
	FPCamera->RelativeLocation = FVector(-28.56, 11.75, 30.f);
	FPCamera->bUsePawnControlRotation = true;

	// Create First Person Mesh
	FPMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FPArms"));
	FPMesh->SetupAttachment(FPCamera);
	FPMesh->SetOnlyOwnerSee(true);
	FPMesh->bCastDynamicShadow = false;
	FPMesh->CastShadow = false;
	FPMesh->RelativeLocation = FVector(-1.604504, -5.647609, -155.168137);
	FPMesh->RelativeRotation = FRotator(1.899995, -19.190002, 5.200006);

	AttachmentPoint = CreateDefaultSubobject<USceneComponent>(TEXT("AttachmentPoint"));
	AttachmentPoint->SetupAttachment(GetMesh());
	AttachmentPoint->AttachToComponent(GetMesh(), FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("AttachmentPoint"));

	//Create Inventory
	InventoryComponent = CreateDefaultSubobject<UInventoryComponent>(TEXT("InventoryComponent"));

}

// Called when the game starts or when spawned
void AMannequin::BeginPlay()
{
	Super::BeginPlay();
	if (GunBlueprint == NULL)
	{
		UE_LOG(LogTemp, Warning, TEXT("No Gun Blueprint"));
		return;
	}

	HeldGun = GetWorld()->SpawnActor<AGun>(GunBlueprint);
	HeldGun->DeactivateRotationComponent();
	HeldGun->DeactivateCapsuleComponent();
	InventoryComponent->AddWeapon(HeldGun);
	if (IsPlayerControlled())
	{
		HeldGun->AttachToComponent(FPMesh, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("GripPoint"));
	}
	else
	{
		HeldGun->AttachToComponent(GetMesh(), FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("GripPoint"));
	}

	HeldGun->FPAnimInstance = FPMesh->GetAnimInstance();
	HeldGun->TPAnimInstance = GetMesh()->GetAnimInstance();

	if (InputComponent != NULL)
	{
		InputComponent->BindAction("Fire", IE_Pressed, this, &AMannequin::PullTrigger);
		InputComponent->BindAction("Fire", IE_Released, this, &AMannequin::ReleaseTrigger);
	}
}

// Called every frame
void AMannequin::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AMannequin::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AMannequin::UnPossessed()
{
	Super::UnPossessed();
	if (HeldGun != nullptr)
	{
		HeldGun->AttachToComponent(GetMesh(), FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("GripPoint"));
	}
}

void AMannequin::PullTrigger()
{
	FVector AimDirection;
	GetAimDirection(AimDirection);

	HeldGun->OnFire(AimDirection);
	ThrowItem(nullptr); //Just For Testing;
}

void AMannequin::ReleaseTrigger()
{
	HeldGun->OnReleaseTrigger();
}

void AMannequin::PickupWeapon(AGun* NewWeapon)
{
	if (IsPlayerControlled() && NewWeapon != nullptr)
	{
		NewWeapon->DeactivateRotationComponent();
		NewWeapon->DeactivateCapsuleComponent();
		NewWeapon->FPAnimInstance = FPMesh->GetAnimInstance();

		if (InventoryComponent->GetNumGunsInInventory() >= MaxGunsInInventory)
		{
			InventoryComponent->RemoveWeapon(HeldGun);
			InventoryComponent->AddWeapon(NewWeapon);
			HeldGun->Destroy();
			HoldWeapon(NewWeapon);
			HeldGun = NewWeapon;
		}
		else
		{
			InventoryComponent->AddWeapon(NewWeapon);
			SwitchHeldWeapon(NewWeapon);
		}
	}
}

void AMannequin::ThrowItem(AActor* Item)
{
	FVector AimDirection;
	if (GetAimDirection(AimDirection))
	{
		FHitResult HitResult;
		auto StartLocation = FPCamera->GetComponentLocation();
		UE_LOG(LogTemp, Warning, TEXT("Start Location is %s"), *StartLocation.ToString());
		auto EndLocation = StartLocation + (AimDirection * 2000); // TODO magic number to be made editable by BP
		if (GetWorld()->LineTraceSingleByChannel(
			HitResult,
			StartLocation,
			EndLocation,
			ECollisionChannel::ECC_Visibility
		))
		{
			UWorld* const World = GetWorld();
			if (World != nullptr)
			{
				auto ThrowStartLocation = FPMesh->GetSocketLocation(FName("ThrowableItem"));
				auto ThrowStartRotation = FPMesh->GetSocketRotation(FName("ThrowableItem"));
				//TODO Item should be a subclass of a ThrowableItem class, the item should be set in Blueprint as the currently equipped throwable item
				auto ThrownItem = World->SpawnActor<AHarpoonTrap>(ThrowableItemClass, ThrowStartLocation, FPCamera->GetComponentRotation());
				ThrownItem->LaunchItem(ThrowStartLocation, HitResult.Location);
			}
		}
	}
}

void AMannequin::SwitchHeldWeapon(AGun* NewHeldWeapon)
{
	StoreWeapon(HeldGun);
	HoldWeapon(NewHeldWeapon);
	//TODO add some sort of animation for switching weapons
}

void AMannequin::HoldWeapon(AGun* Weapon)
{
	if (Weapon != nullptr)
	{
		if (IsPlayerControlled())
		{
			Weapon->AttachToComponent(FPMesh, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("GripPoint"));
		}
		else
		{
			Weapon->AttachToComponent(GetMesh(), FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("GripPoint"));
		}

		HeldGun = Weapon;
	}
}

void AMannequin::StoreWeapon(AGun* Weapon)
{
	if (Weapon != nullptr)
	{
		if (IsPlayerControlled())
		{
			Weapon->AttachToComponent(FPMesh, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("Backpack"));
		}
		else
		{
			Weapon->AttachToComponent(GetMesh(), FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("Backpack"));
		}

		StoredGun = Weapon;
	}
}

bool AMannequin::GetAimDirection(FVector& AimDirection) const
{
	//Find crosshair position from TestingGroundsHUD
	auto HUD = PlayerController->GetHUD();
	ATestingGroundsHUD* TestingGroundsHUD = Cast<ATestingGroundsHUD>(HUD);

	if (TestingGroundsHUD != nullptr)
	{
		auto CrosshairLocation = TestingGroundsHUD->GetCrosshairDrawPosition();

		FVector WorldLocation;
		if (PlayerController != nullptr)
		{
			return PlayerController->DeprojectScreenPositionToWorld(
				CrosshairLocation.X,
				CrosshairLocation.Y,
				WorldLocation,
				AimDirection
			);
		}
	}

	return false;
}

USceneComponent* AMannequin::GetAttachmentPoint() const
{
	return AttachmentPoint;
}