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
	MyDamageCauser = InDamageCauser;
	MyDamage = InDamage;
	MyTriggerTimes = (int32)(InDuration / InTriggerRate);
	GetWorld()->GetTimerManager().SetTimer(MyApplyDamageTimerHandle, this, &UOvertimeDamageComponent::ApplyDamage, InTriggerRate, true);
}

void UOvertimeDamageComponent::ApplyDamage()
{
	UE_LOG(LogTemp, Warning, TEXT("%s: %s dealt %f"), GetWorld()->IsServer() ? TEXT("Server") : TEXT("Client"),  *MyDamageCauser->GetName(), MyDamage);
	UHealthComponent* HealthComp = GetOwner()->FindComponentByClass<UHealthComponent>();
	check(HealthComp != nullptr);
	HealthComp->TakeDamage(MyDamage, MyDamageCauser);

	--MyTriggerTimes;
	if (MyTriggerTimes == 0)
	{
		GetWorld()->GetTimerManager().ClearTimer(MyApplyDamageTimerHandle);
	}
}
