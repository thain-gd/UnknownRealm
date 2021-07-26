// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/PlayerHUDWidget.h"

#include "Components/HealthComponent.h"
#include "Core/MPGameState.h"
#include "Kismet/GameplayStatics.h"
#include "Player/PlayerCharacter.h"

FText UPlayerHUDWidget::GetPreparingTimeText() const
{
	AMPGameState* GameState = GetWorld()->GetGameState<AMPGameState>();
	if (GameState)
	{
		const int RemainingPreparingTime = GameState->GetRemainingPreparingTime();
		const int32 Minutes = RemainingPreparingTime / 60;
		const int32 Seconds = RemainingPreparingTime % 60;
		return FText::FromString(FString::Printf(TEXT("%d:%02d"), Minutes, Seconds));
	}

	return FText();
}

ESlateVisibility UPlayerHUDWidget::GetPreparingTimeVisibility() const
{
	AMPGameState* GameState = GetWorld()->GetGameState<AMPGameState>();
	if (GameState && GameState->GetRemainingPreparingTime() > 0)
	{
		return ESlateVisibility::Visible;
	}
	
	return ESlateVisibility::Hidden;
}

float UPlayerHUDWidget::GetStaminaBarPercent() const
{
	APlayerCharacter* PlayerChar = GetWorld()->GetFirstPlayerController()->GetPawn<APlayerCharacter>();
	if (PlayerChar)
	{
		return PlayerChar->GetStaminaPercent();
	}
	
	return 1.0f;
}

float UPlayerHUDWidget::GetVillageHealthBarPercent() const
{
	AActor* Village = UGameplayStatics::GetActorOfClass(GetWorld(), VillageClass);
	if (IsValid(Village))
	{
		UHealthComponent* HealthComp = Cast<UHealthComponent>(Village->GetComponentByClass(UHealthComponent::StaticClass()));
		if (HealthComp)
		{
			return HealthComp->GetRemainingHealthPercent();
		}
	}

	return 1.0f;
}
