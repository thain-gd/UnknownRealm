// Fill out your copyright notice in the Description page of Project Settings.


#include "Buildings/CraftingObject.h"

// Sets default values
ACraftingObject::ACraftingObject()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	RootComponent = CreateDefaultSubobject<USceneComponent>("DefaultRoot");
	
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetupAttachment(RootComponent);
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

