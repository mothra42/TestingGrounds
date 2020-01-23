// Fill out your copyright notice in the Description page of Project Settings.


#include "Mannequin.h"
#include "Components/CapsuleComponent.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "InventoryComponent.h"
#include "Weapons/Gun.h"

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
	HeldGun->OnFire();
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
			HeldGun = NewWeapon;
			return;
		}

		InventoryComponent->AddWeapon(NewWeapon);
		SwitchHeldWeapon(NewWeapon);
	}
}

void AMannequin::SwitchHeldWeapon(AGun* NewHeldWeapon)
{
	StoreWeapon(HeldGun);
	HoldWeapon(NewHeldWeapon);
}

void AMannequin::HoldWeapon(AGun* Weapon)
{
	Weapon->AttachToComponent(FPMesh, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("GripPoint"));
}

void AMannequin::StoreWeapon(AGun* Weapon)
{
	Weapon->AttachToComponent(FPMesh, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("Backpack"));
	//TODO Third person model does not have a Backpack socket. Needs to be added.
}