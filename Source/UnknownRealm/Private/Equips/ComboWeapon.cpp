#include "Equips/ComboWeapon.h"

#include "Components/ComboComponent.h"
#include "Core/MPGameInstance.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "Player/PlayerCharacter.h"

void AComboWeapon::SetupInputs(UInputComponent* ControllerInputComp)
{
	ControllerInputComp->BindAction("LightAttack", IE_Pressed, this, &AComboWeapon::SR_TriggerLightAttack);
	ControllerInputComp->BindAction("HeavyAttack", IE_Pressed, this, &AComboWeapon::SR_TriggerHeavyAttack);
}

void AComboWeapon::SR_TriggerLightAttack_Implementation()
{
	checkf(ComboComp != nullptr, TEXT("Combo Component hasn't added to this actor yet"));
	
	if (!bIsWeaponActive)
	{
		SetIsWeaponActive(true);
		return;
	}

	// Ignore if there is a waiting attack montage to be played next
	if (NextAttackMontage)
		return;

	UAnimInstance* PlayerAnimInstance = GetOwner<APlayerCharacter>()->GetAnimInstance();
	if (!PlayerAnimInstance->IsAnyMontagePlaying())
	{
		NextAttackMontage = ComboComp->GetNextLightAttackMontage();
		TriggerNextAttack();
	}
	else if (PlayerAnimInstance->Montage_GetCurrentSection() == FName("ComboWindow"))
	{
		NextAttackMontage = ComboComp->GetNextLightAttackMontage();
	}
}

void AComboWeapon::SR_TriggerHeavyAttack_Implementation()
{
	checkf(ComboComp != nullptr, TEXT("Combo Component hasn't added to this actor yet"));

	if (!bIsWeaponActive)
		return;

	// Ignore if there is a waiting attack montage to be played next
	if (NextAttackMontage)
		return;

	UAnimInstance* PlayerAnimInstance = GetOwner<APlayerCharacter>()->GetAnimInstance();
	if (!PlayerAnimInstance->IsAnyMontagePlaying())
	{
		NextAttackMontage = ComboComp->GetNextHeavyAttackMontage();
		TriggerNextAttack();
	}
	else if (PlayerAnimInstance->Montage_GetCurrentSection() == FName("ComboWindow"))
	{
		NextAttackMontage = ComboComp->GetNextHeavyAttackMontage();
	};
}

void AComboWeapon::TriggerNextAttack()
{
	if (HasAuthority() && NextAttackMontage)
	{
		MC_TriggerAttack(NextAttackMontage);
		NextAttackMontage = nullptr;
	}
}

void AComboWeapon::MC_TriggerAttack_Implementation(UAnimMontage* AttackMontage)
{
	GetOwner<APlayerCharacter>()->PlayAnimMontage(AttackMontage);
}

void AComboWeapon::ResetCombo()
{
	if (HasAuthority())
	{
		NextAttackMontage = nullptr;
		DisableAttackCheck();
		ComboComp->Reset();
	}
}

void AComboWeapon::EnableAttackCheck()
{
	MeshComp->SetCollisionProfileName(TEXT("OverlapWeapon"));
}

void AComboWeapon::DisableAttackCheck()
{
	MeshComp->SetCollisionProfileName(TEXT("NoCollision"));
}

void AComboWeapon::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority())
	{
		MeshComp->OnComponentBeginOverlap.AddDynamic(this, &AComboWeapon::OnWeaponHit);
	}
}

void AComboWeapon::OnWeaponHit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!OtherActor->ActorHasTag(FName("AI")))
		return;

	OnEnemyHit(OtherActor);
}
