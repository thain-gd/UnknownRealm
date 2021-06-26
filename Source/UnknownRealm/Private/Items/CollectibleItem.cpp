// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/CollectibleItem.h"

#include "Components/SphereComponent.h"
#include "Components/TextRenderComponent.h"
#include "Core/MPGameState.h"
#include "Core/MPPlayerController.h"
#include "Kismet/GameplayStatics.h"
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
	TriggerSphere->SetMobility(EComponentMobility::Static);
	TriggerSphere->SetSphereRadius(100.0f);
	TriggerSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	TriggerSphere->SetCollisionResponseToAllChannels(ECR_Ignore);
	TriggerSphere->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	TriggerSphere->SetupAttachment(RootComponent);

	bReplicates = true;
}

void ACollectibleItem::OnFinishedCollecting()
{
	AMPGameState* MPGameState = Cast<AMPGameState>(GetWorld()->GetGameState());
	if (MPGameState)
	{
		const int32 CollectedAmount = FMath::RandRange(MinCollectibleAmount, MaxCollectibleAmount);
		MPGameState->AddToInventory(ItemID, CollectedAmount);

		AMPPlayerController* PlayerController = Cast<AMPPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
		if (PlayerController)
		{
			PlayerController->ServerDestroyActor(this);
		}
	}
}
