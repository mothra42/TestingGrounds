// Fill out your copyright notice in the Description page of Project Settings.


#include "Mannequin.h"
#include "Components/CapsuleComponent.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"
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

void AMannequin::PickupWeapon(AGun* NewWeapon)
{
	if (IsPlayerControlled() && NewWeapon != nullptr && HeldGun != NewWeapon)
	{
		UE_LOG(LogTemp, Warning, TEXT("I'm gonna try and pick up that weapon"));
		HeldGun->Destroy();
		HeldGun = NewWeapon;
		NewWeapon->DeactivateRotationComponent();
		NewWeapon->DeactivateCapsuleComponent();
		NewWeapon->AttachToComponent(FPMesh, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("GripPoint"));
		NewWeapon->FPAnimInstance = FPMesh->GetAnimInstance();
	}
}

