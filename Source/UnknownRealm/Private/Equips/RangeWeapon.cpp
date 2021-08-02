// Fill out your copyright notice in the Description page of Project Settings.


#include "Equips/RangeWeapon.h"

#include "Blueprint/UserWidget.h"
#include "Camera/CameraComponent.h"
#include "Components/TimelineComponent.h"
#include "Core/MPGameState.h"
#include "Equips/Projectile.h"
#include "Net/UnrealNetwork.h"
#include "Player/PlayerCharacter.h"
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

	APlayerCharacter* PlayerChar = GetOwner<APlayerCharacter>();
	if (!PlayerChar)
		return;
	
	if (GetOwner<APawn>()->IsLocallyControlled())
	{
		SetupComponents(PlayerChar);
	}
	else
	{
		CL_SetupComponents(PlayerChar);
	}
}

void ARangeWeapon::CL_SetupComponents_Implementation(APlayerCharacter* PlayerChar)
{
	SetupComponents(PlayerChar);
}

void ARangeWeapon::SetupComponents(APlayerCharacter* PlayerChar)
{
	bIsLocallyControlled = true;
	PlayerCharCameraComp = PlayerChar->GetCameraComp();

	SetupChargeTimeline();
}

void ARangeWeapon::SetupChargeTimeline()
{
	// Add update function
	FOnTimelineFloat TimelineProgress;
	TimelineProgress.BindDynamic(this, &ARangeWeapon::SR_UpdateChargeTimelineComp);
	ChargeTimelineComp->AddInterpFloat(ChargeAmountFloatCurve, TimelineProgress);

	// Add finished function
	FOnTimelineEvent ChargingFinishedEvent;
	ChargingFinishedEvent.BindDynamic(this, &ARangeWeapon::OnChargingFinish);
	ChargeTimelineComp->SetTimelineFinishedFunc(ChargingFinishedEvent);
}

void ARangeWeapon::SR_UpdateChargeTimelineComp_Implementation(float NewChargeAmount)
{
	ChargeAmount = NewChargeAmount;
	OR_UpdateTimingMultiplierByChargeAmount();
}

void ARangeWeapon::OR_UpdateTimingMultiplierByChargeAmount()
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

void ARangeWeapon::SetupInputs(UInputComponent* ControllerInputComp)
{
	ControllerInputComp->BindAction("Aim", IE_Pressed, this, &ARangeWeapon::SR_OnAimingStarted);
	ControllerInputComp->BindAction("Aim", IE_Released, this, &ARangeWeapon::SR_OnAimingEnded);
	ControllerInputComp->BindAction("Charge", IE_Pressed, this, &ARangeWeapon::OnChargingStart);
	ControllerInputComp->BindAction("Charge", IE_Released, this, &ARangeWeapon::Fire);
}

void ARangeWeapon::SR_OnAimingStarted_Implementation()
{
	if (!bIsWeaponActive)
		return;

	if (TryReload())
	{
		MC_UpdateIsAiming(true);
	}
	else
	{
		// TODO: Notify no arrow left
	}
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

void ARangeWeapon::Reload()
{
	Arrow = GetWorld()->SpawnActor<AProjectile>(ArrowClass);
	Arrow->SetOwner(GetOwner());
	Arrow->AttachToComponent(SkeletalMeshComp, FAttachmentTransformRules::SnapToTargetIncludingScale, TEXT("ArrowSocket"));
}

void ARangeWeapon::SR_OnAimingEnded_Implementation()
{
	if (!bIsAiming)
		return;

	ResetArrow();
	MC_UpdateIsAiming(false);
}

void ARangeWeapon::ResetArrow()
{
	if (Arrow)
	{
		//StopCharge(); // TODO: Change this to sth else to reset bow animation without TryReload

		GetWorld()->DestroyActor(Arrow);
		Arrow = nullptr;
	}
}

void ARangeWeapon::MC_UpdateIsAiming_Implementation(bool bInIsAiming)
{
	bIsAiming = bInIsAiming;

	APlayerCharacter* PlayerChar = GetOwner<APlayerCharacter>();
	if (bIsAiming)
	{
		PlayerChar->SetMovementForAiming();
		if (bIsLocallyControlled)
		{
			ShowIndicator();
		}
	}
	else
	{
		PlayerChar->ResetMovement();
		if (bIsLocallyControlled)
		{
			HideIndicator();
		}
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

void ARangeWeapon::OnChargingStart()
{
	if (!bIsWeaponActive)
	{
		SR_SetIsWeaponActive(true);
		return;
	}
	
	if (!bIsAiming || !IsValid(Arrow))
		return;

	ChargeTimelineComp->Play();
	BowWidget->StartCharging();
}

void ARangeWeapon::Fire()
{
	if (!IsValid(Arrow))
		return;
	
	BowWidget->StopCharging();
	
	SR_OnFired(TargetLocation, CalculateDamage());

	StopCharge();
}

float ARangeWeapon::CalculateDamage() const
{
	return FMath::RoundToFloat(BaseDmg * RangeMultiplier * TimingMultiplier);
}

void ARangeWeapon::SR_OnFired_Implementation(const FVector& InTargetLocation, const float Damage)
{
	AMPGameState* GameState = GetWorld()->GetGameState<AMPGameState>();
	if (GameState)
	{
		GameState->GetInventory()->RemoveItem(CurrentArrowID);
	}

	Arrow->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	Arrow->OnFired(InTargetLocation, Damage);
	Arrow = nullptr;
}

void ARangeWeapon::StopCharge() const
{
	const float ReversePlaybackTime = 0.1f;
	ChargeTimelineComp->SetNewTime(ReversePlaybackTime);
	ChargeTimelineComp->Reverse();
}

void ARangeWeapon::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (!bIsLocallyControlled || !bIsAiming)
		return;

	UpdateTarget();
}

void ARangeWeapon::UpdateTarget()
{
	const FVector StartLocation = PlayerCharCameraComp->GetComponentLocation();
	const FVector EndLocation = StartLocation + PlayerCharCameraComp->GetForwardVector() * MaxRange;

	FHitResult OutHit;
	TraceHitTarget(OutHit, StartLocation, EndLocation);

	bool bIsTargetEnemy = false;
	float TargetRange;
	if (OutHit.bBlockingHit)
	{
		TargetLocation = OutHit.Location;
		TargetRange = FVector::Distance(StartLocation, TargetLocation);
		bIsTargetEnemy = OutHit.Actor->ActorHasTag(FName("AI"));
	}
	else
	{
		TargetLocation = EndLocation;
		TargetRange = MaxRange;
	}

	UpdateIndicatorByRange(bIsTargetEnemy, TargetRange);
}

void ARangeWeapon::TraceHitTarget(FHitResult& OutHitResult, const FVector& StartLocation,
	const FVector& EndLocation) const
{
	FCollisionQueryParams TraceParams;
	TraceParams.AddIgnoredActor(GetOwner());
	GetWorld()->LineTraceSingleByChannel(OutHitResult, StartLocation, EndLocation, ECC_Visibility, TraceParams);
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

void ARangeWeapon::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(ARangeWeapon, Arrow);
	DOREPLIFETIME(ARangeWeapon, ChargeAmount);
}
