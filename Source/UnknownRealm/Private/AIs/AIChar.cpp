// Fill out your copyright notice in the Description page of Project Settings.


#include "AIs/AIChar.h"

// Sets default values
AAIChar::AAIChar()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

// Called when the game starts or when spawned
void AAIChar::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AAIChar::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

