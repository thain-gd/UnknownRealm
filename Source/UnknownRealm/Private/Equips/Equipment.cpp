#include "Equips/Equipment.h"

#include "GameFramework/Character.h"
#include "Net/UnrealNetwork.h"

AEquipment::AEquipment()
{
	PrimaryActorTick.bCanEverTick = false;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComp"));
	
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetCollisionProfileName(TEXT("NoCollision"));
	MeshComp->SetupAttachment(RootComponent);

	SkeletalMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMeshComp"));
	SkeletalMeshComp->SetCollisionProfileName(TEXT("NoCollision"));
	SkeletalMeshComp->SetupAttachment(RootComponent);

	bReplicates = true;
}

void AEquipment::Init(FEquipmentInfo* InEquipInfo)
{
	EquipInfo = InEquipInfo;
	if (EquipInfo->StaticMesh)
	{
		StaticMesh = EquipInfo->StaticMesh;
		OR_SetStaticMesh();
	}
	else
	{
		SkeletalMesh = EquipInfo->SkeletalMesh;
		OR_SetSkeletalMesh();
	}
}

void AEquipment::OR_SetStaticMesh()
{
	MeshComp->SetStaticMesh(StaticMesh);
	
	SkeletalMeshComp->DestroyComponent();
}

void AEquipment::OR_SetSkeletalMesh()
{
	SkeletalMeshComp->SetSkeletalMesh(SkeletalMesh);

	MeshComp->DestroyComponent();
}

void AEquipment::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AEquipment, SkeletalMesh);
	DOREPLIFETIME(AEquipment, StaticMesh);
}
