// Fill out your copyright notice in the Description page of Project Settings.


#include "Equips/RangeWeapon.h"

#include "Equips/Projectile.h"


ARangeWeapon::ARangeWeapon()
{
	PrimaryActorTick.bCanEverTick = true;

	SkeletalMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMeshComp"));
	RootComponent = SkeletalMeshComp;
}

void ARangeWeapon::Init(FEquipmentInfo* InEquipInfo)
{
	Super::Init(InEquipInfo);

	SkeletalMeshComp->SetSkeletalMesh(InEquipInfo->SkeletalMesh);
}

bool ARangeWeapon::TryReload()
{
	// TODO: Check if there is any arrow before reload
	if (true)
	{
		Reload();
		return true;
	}

	return false;
}

void ARangeWeapon::StopAiming()
{
	GetWorld()->DestroyActor(Arrow);
	Arrow = nullptr;
}

void ARangeWeapon::Reload()
{
	Arrow = GetWorld()->SpawnActor<AProjectile>(ArrowClass);
	Arrow->AttachToComponent(SkeletalMeshComp, FAttachmentTransformRules::SnapToTargetIncludingScale, TEXT("ArrowSocket"));
}
