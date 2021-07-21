// Fill out your copyright notice in the Description page of Project Settings.


#include "Equips/RangeWeapon.h"

#include "Blueprint/UserWidget.h"
#include "Equips/Projectile.h"
#include "Net/UnrealNetwork.h"


ARangeWeapon::ARangeWeapon()
{
	PrimaryActorTick.bCanEverTick = true;

	SkeletalMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMeshComp"));
	SkeletalMeshComp->SetIsReplicated(true);
	RootComponent = SkeletalMeshComp;
}

void ARangeWeapon::Init(FEquipmentInfo* InEquipInfo)
{
	Super::Init(InEquipInfo);

	SkeletalMesh = InEquipInfo->SkeletalMesh;
	OnRepSetMesh();
}

void ARangeWeapon::OnRepSetMesh()
{
	SkeletalMeshComp->SetSkeletalMesh(SkeletalMesh);
}

bool ARangeWeapon::StartAiming()
{
	if (TryReload())
	{
		return true;
	}

	return false;
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
	if (Arrow)
	{
		GetWorld()->DestroyActor(Arrow);
		Arrow = nullptr;
	}
}

void ARangeWeapon::ShowIndicator()
{
	if (!BowWidget)
	{
		BowWidget = CreateWidget<UUserWidget>(GetWorld(), BowWidgetClass);
	}

	BowWidget->AddToViewport();
}

void ARangeWeapon::HideIndicator() const
{
	if (BowWidget)
	{
		BowWidget->RemoveFromViewport();
	}
}

void ARangeWeapon::Fire(const FVector& TargetLocation)
{
	if (!IsValid(Arrow))
		return;

	Arrow->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	Arrow->OnFired(TargetLocation, BaseDmg * 1.5f);
	Arrow = nullptr;
	StopCharge();
}

void ARangeWeapon::Reload()
{
	Arrow = GetWorld()->SpawnActor<AProjectile>(ArrowClass);
	Arrow->SetOwner(GetOwner());
	Arrow->AttachToComponent(SkeletalMeshComp, FAttachmentTransformRules::SnapToTargetIncludingScale, TEXT("ArrowSocket"));
}

void ARangeWeapon::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ARangeWeapon, SkeletalMesh);
}
