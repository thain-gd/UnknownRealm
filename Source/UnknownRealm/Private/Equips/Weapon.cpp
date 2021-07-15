// Fill out your copyright notice in the Description page of Project Settings.


#include "Equips/Weapon.h"

// Sets default values
AWeapon::AWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

FName AWeapon::GetAttachPoint() const
{
	switch (WeaponType)
	{
	case EWeaponType::Bow:
		return TEXT("BowSocket");

	case EWeaponType::Sword:
		return TEXT("SwordSocket");

	case EWeaponType::Spear:
		return TEXT("SpearSocket");

	case EWeaponType::Staff:
		return TEXT("StaffSocket");
	}

	return TEXT("None"); // This should never happen
}

// Called when the game starts or when spawned
void AWeapon::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWeapon::Init(FEquipmentInfo* InEquipInfo)
{
	Super::Init(InEquipInfo);

	WeaponType = StaticCast<FWeaponInfo*>(InEquipInfo)->WeaponType;
}

