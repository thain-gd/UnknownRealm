// Fill out your copyright notice in the Description page of Project Settings.


#include "Equips/Sword.h"

#include "Components/BaseComboComponent.h"
#include "Components/OvertimeDamageComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Player/PlayerCharacter.h"

ASword::ASword()
	: FinalHeavyAttackTriggerTime(0.7f), BleedingDamageModifier(0.05f), BleedingDuration(10.0f), BleedingTriggerRate(1.0f)
{
	WeaponType = EWeaponType::Sword;
	
	ComboComp = CreateDefaultSubobject<UBaseComboComponent>(TEXT("BaseComboComp"));
	AddOwnedComponent(ComboComp);
}

void ASword::CL_SetupInputs_Implementation()
{
	Super::CL_SetupInputs_Implementation();

	if (InputComponent)
	{
		InputComponent->BindAction("CounterAttack", IE_Pressed, this, &ASword::SR_TriggerCounterAttack);
	}
}

void ASword::SR_TriggerCounterAttack_Implementation()
{
	if (!IsReadyToDoCounterAttack())
		return;

	bCanCounterAttack = false; // Reset to prevent multiple counter attacks
	GetOwner<APlayerCharacter>()->MC_PlayMontage(CounterAttackMontage);
}

bool ASword::IsReadyToDoCounterAttack() const
{
	// Can only do counter attack after an attack already starts and currently in recovery frame
	return IsAttacking() && bCanCounterAttack;
}

void ASword::OnEndOverlapWeapon(UPrimitiveComponent* InOverlappedComponent, AActor* InOtherActor, UPrimitiveComponent* InOtherComp, int32 InOtherBodyIndex)
{
	Super::OnEndOverlapWeapon(InOverlappedComponent, InOtherActor, InOtherComp, InOtherBodyIndex);

	if (!FirstHitEnemy)
	{
		StopCheckingFinalHeavyAttack();
	}
}

void ASword::StopCheckingFinalHeavyAttack()
{
	if (HasAuthority())
	{
		GetOwner<APlayerCharacter>()->MC_ResumeMontage();
		GetWorldTimerManager().ClearTimer(FinalHeavyAttackTriggerTimerHandle);
	}
}

void ASword::CheckFinalCounterAttack()
{
	if (HasAuthority() && bCanDoNextCounterStep)
	{
		GetOwner<APlayerCharacter>()->MC_PlayMontage(FinalCounterAttackMontage);
		bCanDoNextCounterStep = false;
	}
}

void ASword::StartCheckingFinalHeavyAttack()
{
	if (!HasAuthority() || !FirstHitEnemy)
	{
		return;
	}
	
	GetOwner<APlayerCharacter>()->MC_PauseMontage();
	
	FTimerDelegate PlayFinalHeavyAttackDelegate;
	PlayFinalHeavyAttackDelegate.BindUObject(GetOwner<APlayerCharacter>(), &APlayerCharacter::MC_PlayMontage, FinalHeavyAttackMontage);
	GetWorldTimerManager().SetTimer(FinalHeavyAttackTriggerTimerHandle, PlayFinalHeavyAttackDelegate, FinalHeavyAttackTriggerTime, false);
}

void ASword::ApplyFinalHeavyAttackEffect()
{
	if (!HasAuthority())
	{
		return;
	}
	
	AActor* DamageReceiver = FirstHitEnemy;
	DisableAttackCheck();
	OnEnemyHit(DamageReceiver);
	ApplyBleedingEffect(DamageReceiver);
	
	// TODO: SFX + Slashing effect when pulling the sword out
}

void ASword::ApplyBleedingEffect(AActor* InDamageReceiver) const
{
	UOvertimeDamageComponent* OvertimeDamageComp = InDamageReceiver->FindComponentByClass<UOvertimeDamageComponent>();
	if (!OvertimeDamageComp)
	{
		OvertimeDamageComp = NewObject<UOvertimeDamageComponent>(InDamageReceiver, UOvertimeDamageComponent::StaticClass());
		check(OvertimeDamageComp != nullptr);
		OvertimeDamageComp->RegisterComponent();
		InDamageReceiver->AddOwnedComponent(OvertimeDamageComp);
	}

	OvertimeDamageComp->Init(GetOwner(), BaseDmg * BleedingDamageModifier, BleedingDuration, BleedingTriggerRate);
}
