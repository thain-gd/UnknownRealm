#include "Equips/ComboWeapon.h"

#include "Components/ComboComponent.h"
#include "Core/MPGameInstance.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "Player/PlayerCharacter.h"

void AComboWeapon::CL_SetupInputs_Implementation()
{
	Super::CL_SetupInputs_Implementation();

	if (InputComponent)
	{
		InputComponent->BindAction("LightAttack", IE_Pressed, this, &AComboWeapon::SR_TriggerLightAttack);
		InputComponent->BindAction("HeavyAttack", IE_Pressed, this, &AComboWeapon::SR_TriggerHeavyAttack);
	}
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
	check(PlayerAnimInstance != nullptr);
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

void AComboWeapon::MC_TriggerAttack_Implementation(UAnimMontage* InAttackMontage)
{
	GetOwner<APlayerCharacter>()->PlayAnimMontage(InAttackMontage);
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
		MeshComp->OnComponentEndOverlap.AddDynamic(this, &AComboWeapon::OnEndOverlapWeapon);
	}
}

void AComboWeapon::OnWeaponHit(UPrimitiveComponent* InOverlappedComponent, AActor* InOtherActor,
	UPrimitiveComponent* InOtherComp, int32 InOtherBodyIndex, bool bInFromSweep, const FHitResult& InSweepResult)
{
	if (!InOtherActor->ActorHasTag(FName("AI")))
		return;

	if (!FirstHitEnemy)
	{
		FirstHitEnemy = InOtherActor;
		OnEnemyHit(InOtherActor);
	}
}

void AComboWeapon::OnEndOverlapWeapon(UPrimitiveComponent* InOverlappedComponent, AActor* InOtherActor, UPrimitiveComponent* InOtherComp, int32 InOtherBodyIndex)
{
	if (!InOtherActor->ActorHasTag(FName("AI")) || InOtherActor != FirstHitEnemy)
		return;

	FirstHitEnemy = nullptr;
}