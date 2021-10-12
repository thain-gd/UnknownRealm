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
	
	if (MyOvertimeDamageInfoByActorMap.Contains(InDamageCauser))
	{
		MyOvertimeDamageInfoByActorMap[InDamageCauser].MyTriggerTimes = TriggerTimes;
		GetWorld()->GetTimerManager().SetTimer(MyOvertimeDamageInfoByActorMap[InDamageCauser].MyApplyDamageTimerHandle, ApplyDamageDelegate, InTriggerRate, true);
	}
	else
	{
		FOvertimeDamageInfo OvertimeDamageInfo{ InDamage, TriggerTimes };
		GetWorld()->GetTimerManager().SetTimer(OvertimeDamageInfo.MyApplyDamageTimerHandle, ApplyDamageDelegate, InTriggerRate, true);
		MyOvertimeDamageInfoByActorMap.Add(InDamageCauser, OvertimeDamageInfo);
	}
}

void UOvertimeDamageComponent::ApplyDamage(AActor* InDamageCauser)
{
	FOvertimeDamageInfo& OvertimeDamageInfo = MyOvertimeDamageInfoByActorMap[InDamageCauser];
	UE_LOG(LogTemp, Warning, TEXT("%s dealt %f"), *InDamageCauser->GetName(), OvertimeDamageInfo.MyDamage);
	UHealthComponent* HealthComp = GetOwner()->FindComponentByClass<UHealthComponent>();
	check(HealthComp != nullptr);
	HealthComp->TakeDamage(OvertimeDamageInfo.MyDamage, MyDamageCauser);

	--OvertimeDamageInfo.MyTriggerTimes;
	if (OvertimeDamageInfo.MyTriggerTimes == 0)
	{
		GetWorld()->GetTimerManager().ClearTimer(OvertimeDamageInfo.MyApplyDamageTimerHandle);
	}
}
