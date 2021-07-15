#include "Equips/Equipment.h"

AEquipment::AEquipment()
{
	PrimaryActorTick.bCanEverTick = false;

	bReplicates = true;
}

void AEquipment::Init(FEquipmentInfo* InEquipInfo)
{
	EquipInfo = InEquipInfo;
}

FName AEquipment::GetAttachPoint() const
{
	switch (EquipInfo->Category)
	{
	case EEquipmentCategory::Helm:
		return TEXT("HelmSocket");

	case EEquipmentCategory::Chest:
		return TEXT("ChestSocket");

	case EEquipmentCategory::Gloves:
		return TEXT("GlovesSocket");

	case EEquipmentCategory::Leggings:
		return TEXT("LeggingsSocket");

	// Weapon category will override its own GetAttachPoint
	default:
		return TEXT("None");
	}
}
