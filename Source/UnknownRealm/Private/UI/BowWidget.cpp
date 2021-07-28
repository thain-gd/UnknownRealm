// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/BowWidget.h"

#include "Components/Image.h"
#include "Equips/RangeWeapon.h"

void UBowWidget::StartCharging()
{
	if (CurrRangeState == ERangeState::OutOfRange)
		return;
	
	PlayAnimation(Charging);
}

void UBowWidget::StopCharging()
{
	StopAnimation(Charging);
	ResetCrosshair();
}

void UBowWidget::UpdateCrosshair(ERangeState RangeState)
{
	UTexture2D* CrosshairTexture = nullptr;
	CurrRangeState = RangeState;
	switch (CurrRangeState)
	{
	case ERangeState::OutOfRange:
		CrosshairTexture = OutOfRangeCrosshairTexture;
		break;

	case ERangeState::Normal:
		CrosshairTexture = NormalRangeCrosshairTexture;
		break;

	case ERangeState::Optimal:
		CrosshairTexture = OptimalRangeCrosshairTexture;
		break;
	}

	Crosshair->SetBrushFromTexture(CrosshairTexture);
}

void UBowWidget::ResetCrosshair()
{
	TimingRadius->SetVisibility(ESlateVisibility::Hidden);
	Crosshair->SetColorAndOpacity(FLinearColor::White);
}
