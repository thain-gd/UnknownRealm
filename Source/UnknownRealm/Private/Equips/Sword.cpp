// Fill out your copyright notice in the Description page of Project Settings.


#include "Equips/Sword.h"

#include "Components/BaseComboComponent.h"
#include "Components/OvertimeDamageComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Player/PlayerCharacter.h"

ASword::ASword()
{
	WeaponType = EWeaponType::Sword;
	
	ComboComp = CreateDefaultSubobject<UBaseComboComponent>(TEXT("BaseComboComp"));
	AddOwnedComponent(ComboComp);
}

void ASword::SetupInputs(UInputComponent* ControllerInputComp)
{
	Super::SetupInputs(ControllerInputComp);

	ControllerInputComp->BindAction("CounterAttack", IE_Pressed, this, &ASword::SR_TriggerCounterAttack);
}

void ASword::SR_TriggerCounterAttack_Implementation()
{
	if (!IsReadyToDoCounterAttack())
		return;

	bCanCounterAttack = false; // Reset to prevent multiple counter attacks
	GetOwner<APlayerCharacter>()->PlayAnimMontage(CounterAttackMontage);
}

bool ASword::IsReadyToDoCounterAttack() const
{
	// Can only do counter attack after an attack already starts and currently in recovery frame
	return IsAttacking() && bCanCounterAttack;
}

void ASword::OnEndOverlapWeapon(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Super::OnEndOverlapWeapon(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);

	if (!FirstHitEnemy)
	{
		StopCheckingLastHAttackStep();
	}
}

void ASword::StopCheckingLastHAttackStep()
{
	GetWorldTimerManager().ClearTimer(LastHAttackStepTriggerTimerHandle);
	UAnimInstance* PlayerAnimInstance = GetOwner<APlayerCharacter>()->GetAnimInstance();
	check(PlayerAnimInstance != nullptr);
	if (PlayerAnimInstance)
	{
		PlayerAnimInstance->Montage_Resume(nullptr);
	}
}

void ASword::StartCheckingLastHAttackStep()
{
	if (!FirstHitEnemy)
		return;

	UAnimInstance* PlayerAnimInstance = GetOwner<APlayerCharacter>()->GetAnimInstance();
	check(PlayerAnimInstance != nullptr);
	if (PlayerAnimInstance)
	{
		PlayerAnimInstance->Montage_Pause();
		GetWorldTimerManager().SetTimer(LastHAttackStepTriggerTimerHandle, this, &ASword::PlayLastHAttackStep, TimeBeforeTriggerLastHAttackStep, false);
	}
}

void ASword::PlayLastHAttackStep() const
{
	GetOwner<APlayerCharacter>()->PlayAnimMontage(LastHAttackMontage);
}

void ASword::ApplyLastHAttackEffect()
{
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
