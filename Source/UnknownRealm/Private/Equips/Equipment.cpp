#include "Equips/Equipment.h"

#include "Net/UnrealNetwork.h"

AEquipment::AEquipment()
{
	PrimaryActorTick.bCanEverTick = false;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	SkeletalMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMeshComp"));

	bReplicates = true;
}

void AEquipment::Init(FEquipmentInfo* InEquipInfo)
{
	EquipInfo = InEquipInfo;
	if (EquipInfo->StaticMesh)
	{
		RootComponent = MeshComp;
		SkeletalMeshComp->DestroyComponent();
		
		StaticMesh = EquipInfo->StaticMesh;
		OR_SetStaticMesh();
	}
	else
	{
		RootComponent = SkeletalMeshComp;
		MeshComp->DestroyComponent();
		
		SkeletalMesh = EquipInfo->SkeletalMesh;
		OR_SetSkeletalMesh();
	}
}

void AEquipment::OR_SetStaticMesh() const
{
	MeshComp->SetStaticMesh(StaticMesh);
}

void AEquipment::OR_SetSkeletalMesh() const
{
	SkeletalMeshComp->SetSkeletalMesh(SkeletalMesh);
}

void AEquipment::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AEquipment, SkeletalMesh);
	DOREPLIFETIME(AEquipment, StaticMesh);
}
