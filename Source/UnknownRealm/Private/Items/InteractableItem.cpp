// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/InteractableItem.h"

#include "Components/BoxComponent.h"
#include "Components/TextRenderComponent.h"
#include "Core/MPPlayerController.h"
#include "Player/PlayerCharacter.h"

// Sets default values
AInteractableItem::AInteractableItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetupAttachment(RootComponent);

	TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
	TriggerBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	TriggerBox->SetCollisionResponseToAllChannels(ECR_Ignore);
	TriggerBox->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	TriggerBox->SetupAttachment(RootComponent);

	TriggeredTextRender = CreateDefaultSubobject<UTextRenderComponent>(TEXT("TriggeredTextRender"));
	TriggeredTextRender->SetVisibility(false);
	TriggeredTextRender->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AInteractableItem::BeginPlay()
{
	Super::BeginPlay();

	TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &AInteractableItem::ShowInteractingUI);
	TriggerBox->OnComponentEndOverlap.AddDynamic(this, &AInteractableItem::HideInteractingUI);
}

void AInteractableItem::ShowInteractingUI(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	APlayerCharacter* PlayerChar = Cast<APlayerCharacter>(OtherActor);
	if (PlayerChar && PlayerChar->GetController()->IsLocalController())
	{
		TriggeredTextRender->SetText(FText::FromString(FString::Printf(TEXT("E - %s"), *InteractString)));
		TriggeredTextRender->SetVisibility(true);
	}
}

void AInteractableItem::HideInteractingUI(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	APlayerCharacter* PlayerChar = Cast<APlayerCharacter>(OtherActor);
	if (PlayerChar && PlayerChar->GetController()->IsLocalController())
	{
		TriggeredTextRender->SetVisibility(false);
	}
}
