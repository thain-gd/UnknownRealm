// Fill out your copyright notice in the Description page of Project Settings.


#include "Equips/RangeWeapon.h"

#include "Blueprint/UserWidget.h"
#include "Components/TimelineComponent.h"
#include "Core/MPGameState.h"
#include "Equips/Projectile.h"
#include "Net/UnrealNetwork.h"
#include "UI/BowWidget.h"


ARangeWeapon::ARangeWeapon()
{
	PrimaryActorTick.bCanEverTick = true;

	ChargeTimelineComp = CreateDefaultSubobject<UTimelineComponent>(TEXT("ChargeTimelineComp"));

	TimingMultiplier = DefaultTimingMultiplier;
}

void ARangeWeapon::Init(FEquipmentInfo* InEquipInfo)
{
	Super::Init(InEquipInfo);

	ClientSetupChargeTimeline();
}

void ARangeWeapon::ClientSetupChargeTimeline_Implementation() const
{
	// Add update function
	FOnTimelineFloat TimelineProgress;
	TimelineProgress.BindDynamic(this, &ARangeWeapon::ServerUpdateChargeTimelineComp);
	ChargeTimelineComp->AddInterpFloat(ChargeAmountFloatCurve, TimelineProgress);

	// Add finished function
	FOnTimelineEvent ChargingFinishedEvent;
	ChargingFinishedEvent.BindDynamic(this, &ARangeWeapon::OnChargingFinish);
	ChargeTimelineComp->SetTimelineFinishedFunc(ChargingFinishedEvent);
}

void ARangeWeapon::ServerUpdateChargeTimelineComp_Implementation(float NewChargeAmount)
{
	ChargeAmount = NewChargeAmount;
	OnRepUpdateTimingMultiplierByChargeAmount();
}

bool ARangeWeapon::StartAiming()
{
	if (TryReload())
	{
		return true;
	}

	return false;
}

bool ARangeWeapon::TryReload()
{
	AMPGameState* GameState = GetWorld()->GetGameState<AMPGameState>();
	if (true)//GameState && GameState->GetInventory()->AreItemsAvailable({{ CurrentArrowID, 1 }}))
	{
		Reload();
		return true;
	}

	// TODO: Notify player about not enough arrow
	UE_LOG(LogTemp, Warning, TEXT("Not enough arrow!"));
	return false;
}

void ARangeWeapon::StopAiming()
{
	if (Arrow)
	{
		//StopCharge(); // TODO: Change this to sth else to reset bow animation without TryReload
		
		GetWorld()->DestroyActor(Arrow);
		Arrow = nullptr;
	}
}

void ARangeWeapon::ShowIndicator()
{
	if (!BowWidget)
	{
		BowWidget = CreateWidget<UBowWidget>(GetWorld(), BowWidgetClass);
	}

	BowWidget->AddToViewport();
}

void ARangeWeapon::HideIndicator() const
{
	if (BowWidget)
	{
		BowWidget->RemoveFromViewport();
	}
}

void ARangeWeapon::UpdateIndicatorByRange(bool bIsTargetEnemy, float CurrentRange)
{
	// Out of range state by default and change only if the target is an enemy and within range
	RangeMultiplier = 0.0f;
	ERangeState RangeState = ERangeState::OutOfRange;
	if (bIsTargetEnemy)
	{
		if (OptimalRangeLowerBound <= CurrentRange && CurrentRange <= OptimalRangeUpperBound)
		{
			RangeState = ERangeState::Optimal;
			RangeMultiplier = OptimalRangeMultiplier;
		}
		else if (CurrentRange < OptimalRangeLowerBound || CurrentRange > OptimalRangeUpperBound && CurrentRange <= MaxRange)
		{
			RangeState = ERangeState::Normal;
			RangeMultiplier = AcceptableRangeMultiplier;
		}
	}

	BowWidget->UpdateCrosshair(RangeState);
}

void ARangeWeapon::OnRepUpdateTimingMultiplierByChargeAmount()
{
	if (ChargeAmount >= 0.999f)
	{
		UpdateTimingMultiplier(ETimingState::Perfect);
	}
	else if (ChargeAmount >= 0.85f)
	{
		UpdateTimingMultiplier(ETimingState::Good);
	}
	else
	{
		UpdateTimingMultiplier(ETimingState::Default);
	}
}

void ARangeWeapon::UpdateTimingMultiplier(ETimingState TimingState)
{
	switch (TimingState)
	{
	case ETimingState::Default:
		TimingMultiplier = DefaultTimingMultiplier;
		break;

	case ETimingState::Good:
		TimingMultiplier = GoodTimingMultiplier;
		break;

	case ETimingState::Perfect:
		TimingMultiplier = PerfectTimingMultiplier;
		break;
	}
}

void ARangeWeapon::OnChargingStart() const
{
	if (!IsValid(Arrow))
		return;

	ChargeTimelineComp->Play();
	BowWidget->StartCharging();
}

void ARangeWeapon::Fire(const FVector& TargetLocation)
{
	if (!IsValid(Arrow))
		return;
	
	BowWidget->StopCharging();
	
	ServerOnFired(TargetLocation, GetDamage());

	StopCharge();
}

void ARangeWeapon::StopCharge() const
{
	const float ReversePlaybackTime = 0.1f;
	ChargeTimelineComp->SetNewTime(ReversePlaybackTime);
	ChargeTimelineComp->Reverse();
}

float ARangeWeapon::GetDamage() const
{
	return FMath::RoundToFloat(BaseDmg * RangeMultiplier * TimingMultiplier);
}

void ARangeWeapon::ServerOnFired_Implementation(const FVector& TargetLocation, const float Damage)
{
	AMPGameState* GameState = GetWorld()->GetGameState<AMPGameState>();
	if (GameState)
	{
		GameState->GetInventory()->RemoveItem(CurrentArrowID);
	}

	Arrow->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	Arrow->OnFired(TargetLocation, Damage);
	Arrow = nullptr;
}

void ARangeWeapon::Reload()
{
	Arrow = GetWorld()->SpawnActor<AProjectile>(ArrowClass);
	Arrow->SetOwner(GetOwner());
	Arrow->AttachToComponent(SkeletalMeshComp, FAttachmentTransformRules::SnapToTargetIncludingScale, TEXT("ArrowSocket"));
}

void ARangeWeapon::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(ARangeWeapon, Arrow);
	DOREPLIFETIME(ARangeWeapon, ChargeAmount);
}
