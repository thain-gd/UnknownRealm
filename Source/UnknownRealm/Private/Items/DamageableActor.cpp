// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/DamageableActor.h"

#include "Components/HealthComponent.h"

// Sets default values
ADamageableActor::ADamageableActor()
{
	RootComponent = CreateDefaultSubobject<USceneComponent>("Root");
	
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetupAttachment(RootComponent);

	HealthComp = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComp"));
	AddOwnedComponent(HealthComp);
}

void ADamageableActor::BeginPlay()
{
	Super::BeginPlay();

	HealthComp->OnHealthChanged.BindDynamic(this, &ADamageableActor::OnHealthChanged);
}

void ADamageableActor::OnHealthChanged()
{
}

