// Fill out your copyright notice in the Description page of Project Settings.


#include "Equips/Weapon.h"

#include "Combat/BaseDamageType.h"
#include "Core/MPGameInstance.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"

const FName AWeapon::InactiveSocketName = FName("InactiveWeaponSocket");

// Sets default values
AWeapon::AWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

void AWeapon::Init(FEquipmentInfo* InEquipInfo)
{
	Super::Init(InEquipInfo);

	AttachToComponent(GetOwner<ACharacter>()->GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, InactiveSocketName);
	
	FWeaponInfo* WeaponInfo = StaticCast<FWeaponInfo*>(InEquipInfo);
	BaseDmg = WeaponInfo->BaseDmg;
}

void AWeapon::CL_SetupInputs_Implementation()
{
	InputComponent = NewObject<UInputComponent>(this);
	InputComponent->RegisterComponent();
	if (InputComponent)
	{
		EnableInput(GetWorld()->GetFirstPlayerController());
	}
}

void AWeapon::SR_SetIsWeaponActive_Implementation(bool bInIsWeaponActive)
{
	SetIsWeaponActive(bInIsWeaponActive);
}

void AWeapon::SetIsWeaponActive(bool bInIsWeaponActive)
{
	// TODO: Switch to using weapon animation
	if (bInIsWeaponActive)
	{
		AttachToComponent(GetOwner<ACharacter>()->GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, ActiveSocketName);
	}
	else
	{
		AttachToComponent(GetOwner<ACharacter>()->GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, InactiveSocketName);
	}

	bIsWeaponActive = bInIsWeaponActive;
}

void AWeapon::SetMotionValue(float InMotionValue)
{
	CurrentMotionValue = InMotionValue;
}

void AWeapon::OnEnemyHit(AActor* InEnemy)
{
	const int32 TotalDmg = GetTotalDmg();
	UGameplayStatics::ApplyDamage(InEnemy, TotalDmg, nullptr, GetOwner(), UBaseDamageType::StaticClass());
}

int32 AWeapon::GetTotalDmg() const
{
	return FMath::RoundToInt(BaseDmg * CurrentMotionValue);
}

void AWeapon::CL_ShowDmgDealt_Implementation(int32 InTotalDmg)
{
	GetGameInstance<UMPGameInstance>()->ShowDamage(InTotalDmg);
}

void AWeapon::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AWeapon, BaseDmg);
	DOREPLIFETIME(AWeapon, bIsWeaponActive);
}
