// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/CollectibleItem.h"

#include "Components/SphereComponent.h"
#include "Components/TextRenderComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Player/PlayerCharacter.h"

// Sets default values
ACollectibleItem::ACollectibleItem()
	: MinCollectibleAmount(1), MaxCollectibleAmount(1)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetupAttachment(RootComponent);

	TriggerSphere = CreateDefaultSubobject<USphereComponent>(TEXT("TriggerSphere"));
	TriggerSphere->SetSphereRadius(100.0f);
	TriggerSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	TriggerSphere->SetCollisionResponseToAllChannels(ECR_Ignore);
	TriggerSphere->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	TriggerSphere->SetupAttachment(RootComponent);
}

void ACollectibleItem::OnFinishedCollecting()
{
	// TODO: Add into inventory
	const int32 CollectedAmount = FMath::RandRange(MinCollectibleAmount, MaxCollectibleAmount);
	UE_LOG(LogTemp, Warning, TEXT("You received %i"), CollectedAmount);
	
	GetWorld()->DestroyActor(this);
}
