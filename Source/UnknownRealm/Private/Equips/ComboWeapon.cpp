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

	SetNextAttackMontage(ComboComp->GetNextLightAttackMontage());
}

void AComboWeapon::SR_TriggerHeavyAttack_Implementation()
{
	checkf(ComboComp != nullptr, TEXT("Combo Component hasn't added to this actor yet"));

	if (!bIsWeaponActive)
	{
		return;
	}

	SetNextAttackMontage(ComboComp->GetNextHeavyAttackMontage());
}

void AComboWeapon::SetNextAttackMontage(UAnimMontage* InNextMontage)
{
	// Ignore if there is a waiting attack montage to be played next
	if (NextAttackMontage)
	{
		return;
	}

	APlayerCharacter* PlayerOwner = GetOwner<APlayerCharacter>();
	check(PlayerOwner);
	if (!PlayerOwner->IsAnyMontagePlaying())
	{
		NextAttackMontage = InNextMontage;
		TriggerNextAttack();
	}
	else if (PlayerOwner->CheckMontageSection("ComboWindow"))
	{
		NextAttackMontage = InNextMontage;
	}
}

void AComboWeapon::TriggerNextAttack()
{
	if (HasAuthority() && NextAttackMontage)
	{
		GetOwner<APlayerCharacter>()->MC_PlayMontage(NextAttackMontage);
		NextAttackMontage = nullptr;
	}
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