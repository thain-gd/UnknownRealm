// Fill out your copyright notice in the Description page of Project Settings.


#include "Buildings/Village.h"

#include "Components/HealthComponent.h"
#include "Core/MPGameMode.h"
#include "Kismet/GameplayStatics.h"

void AVillage::OnHealthChanged()
{
	if (!HealthComp->IsAlive())
	{
		AMPGameMode* MPGameMode = Cast<AMPGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
		if (MPGameMode)
		{
			MPGameMode->SetWaveStatus(false);
		}
	}
}
