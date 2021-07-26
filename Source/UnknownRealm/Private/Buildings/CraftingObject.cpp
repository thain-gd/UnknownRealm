// Fill out your copyright notice in the Description page of Project Settings.


#include "Buildings/CraftingObject.h"

#include "Landscape.h"
#include "Components/BoxComponent.h"
#include "Net/UnrealNetwork.h"

// Sets default values
ACraftingObject::ACraftingObject()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	RootComponent = CreateDefaultSubobject<USceneComponent>("Root");
	
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetCollisionResponseToAllChannels(ECR_Overlap);
	MeshComp->SetupAttachment(RootComponent);

	BuildCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BuildCollision"));
	BuildCollision->SetGenerateOverlapEvents(true);
	BuildCollision->SetCollisionResponseToAllChannels(ECR_Overlap);
	BuildCollision->SetupAttachment(RootComponent);

	FoundationCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("Foundation"));
	FoundationCollision->SetGenerateOverlapEvents(true);
	FoundationCollision->SetCollisionResponseToAllChannels(ECR_Overlap);
	FoundationCollision->SetupAttachment(RootComponent);

	bReplicates = true;
	SetReplicatingMovement(true);
}

// Called when the game starts or when spawned
void ACraftingObject::BeginPlay()
{
	Super::BeginPlay();

	SaveDefaultMaterials();
}

void ACraftingObject::SaveDefaultMaterials()
{
	for (size_t i = 0; i < MeshComp->GetMaterials().Num(); ++i)
	{
		DefaultMaterials.Add(MeshComp->GetMaterial(i));
	}
}

bool ACraftingObject::CheckBuildStatus()
{
	TArray<AActor*> FoundationOverlappingActors;
	FoundationCollision->GetOverlappingActors(FoundationOverlappingActors);
	bool bOnGround = false;
	for (AActor* Actor : FoundationOverlappingActors)
	{
		if (IsLandScape(Actor))
		{
			bOnGround = true;
			break;
		}
	}

	if (!bOnGround)
	{
		return false;
	}

	TArray<AActor*> OverlappingActors;
	BuildCollision->GetOverlappingActors(OverlappingActors);
	int32 RelevantOverlappingActorCount = OverlappingActors.Num();
	for (AActor* Actor : OverlappingActors)
	{
		bool bNotBlocked = true;
		if (Actor != this && !IsLandScape(Actor))
		{
			TArray<UPrimitiveComponent*> OverlappingComponents;
			Actor->GetOverlappingComponents(OverlappingComponents);
			for (auto Component : OverlappingComponents)
			{
				if ((Component->GetCollisionResponseToChannel(BuildCollision->GetCollisionObjectType()) == ECR_Block) ||
					(Component->GetOwner() == Actor))
				{
					bNotBlocked = false;
					break;
				}
			}
		}
		
		if (bNotBlocked)
			--RelevantOverlappingActorCount;
	}

	if (RelevantOverlappingActorCount == 0)
	{
		bIsBuildable = true;
	}
	else
	{
		bIsBuildable = false;
	}

	return bIsBuildable;
}

// Called every frame
void ACraftingObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACraftingObject::Init(UMaterialInstance* NewMaterial)
{
	MulticastSetMaterials(NewMaterial);
}

void ACraftingObject::SetBuildability(bool bInIsBuidable, UMaterialInstance* NewMaterial)
{
	bIsBuildable = bInIsBuidable;
	MulticastSetMaterials(NewMaterial);
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

bool ACraftingObject::IsLandScape(AActor* Actor) const
{
	return Actor->GetClass() == ALandscape::StaticClass() || Actor->ActorHasTag(FName("Landscape"));
}

void ACraftingObject::MulticastConfirmPlacement_Implementation() const
{
	MeshComp->SetCollisionResponseToAllChannels(ECR_Block);

	for (size_t i = 0; i < MeshComp->GetMaterials().Num(); ++i)
	{
		MeshComp->SetMaterial(i, DefaultMaterials[i]);
	}
}

void ACraftingObject::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ACraftingObject, bIsBuildable);
}
