// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/OvertimeDamageComponent.h"

#include "Components/HealthComponent.h"

// Sets default values for this component's properties
UOvertimeDamageComponent::UOvertimeDamageComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
}

void UOvertimeDamageComponent::Init(AActor* InDamageCauser, float InDamage, float InDuration, float InTriggerRate)
{
	const int32 TriggerTimes = (int32)(InDuration / InTriggerRate);
	FTimerDelegate ApplyDamageDelegate;
	ApplyDamageDelegate.BindUFunction(this, FName("ApplyDamage"), InDamageCauser);
	
	if (OvertimeDamageInfoByActorMap.Contains(InDamageCauser))
	{
		OvertimeDamageInfoByActorMap[InDamageCauser].TriggerTimes = TriggerTimes;
		GetWorld()->GetTimerManager().SetTimer(OvertimeDamageInfoByActorMap[InDamageCauser].ApplyDamageTimerHandle, ApplyDamageDelegate, InTriggerRate, true);
	}
	else
	{
		FOvertimeDamageInfo OvertimeDamageInfo{ InDamage, TriggerTimes };
		GetWorld()->GetTimerManager().SetTimer(OvertimeDamageInfo.ApplyDamageTimerHandle, ApplyDamageDelegate, InTriggerRate, true);
		OvertimeDamageInfoByActorMap.Add(InDamageCauser, OvertimeDamageInfo);
	}
}

void UOvertimeDamageComponent::ApplyDamage(AActor* InDamageCauser)
{
	FOvertimeDamageInfo& OvertimeDamageInfo = OvertimeDamageInfoByActorMap[InDamageCauser];
	UE_LOG(LogTemp, Warning, TEXT("%s dealt %f"), *InDamageCauser->GetName(), OvertimeDamageInfo.Damage);
	UHealthComponent* HealthComp = GetOwner()->FindComponentByClass<UHealthComponent>();
	check(HealthComp != nullptr);
	HealthComp->TakeDamage(OvertimeDamageInfo.Damage, DamageCauser);

	--OvertimeDamageInfo.TriggerTimes;
	if (OvertimeDamageInfo.TriggerTimes == 0)
	{
		GetWorld()->GetTimerManager().ClearTimer(OvertimeDamageInfo.ApplyDamageTimerHandle);
	}
}
