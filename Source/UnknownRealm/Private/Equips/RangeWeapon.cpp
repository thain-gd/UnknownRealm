// Fill out your copyright notice in the Description page of Project Settings.


#include "Equips/RangeWeapon.h"


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
