// Fill out your copyright notice in the Description page of Project Settings.


#include "Buildings/Village.h"

#include "DrawDebugHelpers.h"
#include "Components/HealthComponent.h"
#include "Core/MPGameMode.h"
#include "Kismet/GameplayStatics.h"

AVillage::AVillage()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AVillage::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (HasAuthority())
	{
		for (const FVector& TargetableLocation : MyTargetableLocations)
		{
			DrawDebugSphere(GetWorld(), TargetableLocation, 20.0f, 12, FColor::White);
		}
	}
}

FVector AVillage::GetClosestTargetableLocation(const FVector& InAILocation)
{
	float SmallestDistSquared = TNumericLimits<float>::Max();
	FVector ClosestTargetableLocation;
	for (const FVector& TargetableLocation : MyTargetableLocations)
	{
		const float DistSquared = FVector::DistSquared(InAILocation, TargetableLocation);
		if (DistSquared < SmallestDistSquared)
		{
			SmallestDistSquared = DistSquared;
			ClosestTargetableLocation = TargetableLocation;
		}
	}

	MyTargetableLocations.Remove(ClosestTargetableLocation);
	return ClosestTargetableLocation;
}

void AVillage::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority())
	{
		if (bMyIsCircleBase)
		{
			const float Radius = MeshComp->Bounds.BoxExtent.X + 10.0f;
			const float Angle = FMath::DegreesToRadians(360.0f / MyTargetableLocationCount);
			for (int i = 0; i < MyTargetableLocationCount; i++)
			{
				MyTargetableLocations.Add(GetActorLocation() + FVector(Radius * FMath::Cos(Angle * i), Radius * FMath::Sin(Angle * i), 0));
			}
		}
		else
		{
			const FVector Offset = MeshComp->Bounds.BoxExtent + FVector::OneVector * 15.0f;
			for (int i = -1; i <= 1; i++)
			{
				for (int j = -1; j <= 1; j++)
				{
					if (i != 0 || j != 0)
					{
						MyTargetableLocations.Add(GetActorLocation() + FVector(Offset.X * i, Offset.Y * j, 0));
					}
				}
			}
		}
	}
}

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
