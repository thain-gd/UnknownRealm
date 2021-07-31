// Fill out your copyright notice in the Description page of Project Settings.


#include "Equips/Weapon.h"

#include "Net/UnrealNetwork.h"

// Sets default values
AWeapon::AWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

void AWeapon::Init(FEquipmentInfo* InEquipInfo)
{
	Super::Init(InEquipInfo);

	FWeaponInfo* WeaponInfo = StaticCast<FWeaponInfo*>(InEquipInfo);
	BaseDmg = WeaponInfo->BaseDmg;
}

void AWeapon::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AWeapon, BaseDmg);
}
