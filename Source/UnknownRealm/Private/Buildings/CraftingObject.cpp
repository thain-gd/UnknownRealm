// Fill out your copyright notice in the Description page of Project Settings.


#include "Buildings/CraftingObject.h"

#include "Net/UnrealNetwork.h"

// Sets default values
ACraftingObject::ACraftingObject()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	RootComponent = CreateDefaultSubobject<USceneComponent>("Root");
	
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetupAttachment(RootComponent);
	
	SetReplicates(true);
	SetReplicatingMovement(true);
}

// Called when the game starts or when spawned
void ACraftingObject::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ACraftingObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACraftingObject::MulticastInit_Implementation(UMaterialInstance* NewMaterial) const
{
	MeshComp->SetCollisionResponseToAllChannels(ECR_Overlap);
	SetMaterials(NewMaterial);
}

void ACraftingObject::MulticastSetMaterials_Implementation(UMaterialInstance* NewMaterial) const
{
	SetMaterials(NewMaterial);
}

void ACraftingObject::SetMaterials(UMaterialInstance* NewMaterial) const
{
	for (size_t i = 0; i < MeshComp->GetMaterials().Num(); ++i)
	{
		MeshComp->SetMaterial(i, NewMaterial);
	}
}

