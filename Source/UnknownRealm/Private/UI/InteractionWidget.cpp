// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/InteractionWidget.h"

#include "Blueprint/WidgetTree.h"
#include "Components/CanvasPanel.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Items/CollectibleItem.h"
#include "Player/PlayerCharacter.h"

UInteractionWidget::UInteractionWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UInteractionWidget::SetInteractText(const FString& ActionString)
{
	if (InteractText)
	{
		const FString InteractString = FString("E - ") + ActionString;
		InteractText->SetText(FText::FromString(InteractString));
	}
}

void UInteractionWidget::OnStartCollecting(ACollectibleItem* CollectibleItem)
{
	CollectibleActor = CollectibleItem;
	InteractText->SetText(FText::FromString("E - Cancel"));
	RemainingCollectionTime = CollectibleActor->GetCollectionTime();
	
	CollectingBar->SetPercent(0.0f);
	CollectingBar->SetVisibility(ESlateVisibility::Visible);
}

void UInteractionWidget::OnFinishCollecting()
{
	CollectingBar->SetVisibility(ESlateVisibility::Hidden);
	
	CollectibleActor->OnFinishedCollecting();
	CollectibleActor = nullptr;
}

void UInteractionWidget::CancelCollecting() const
{
	CollectingBar->SetVisibility(ESlateVisibility::Hidden);

	const FString InteractString = FString("E - ") + CollectibleActor->GetInteractString();
	InteractText->SetText(FText::FromString(InteractString));
}

void UInteractionWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UInteractionWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (CollectingBar && CollectingBar->GetVisibility() == ESlateVisibility::Visible)
	{
		RemainingCollectionTime -= InDeltaTime;
		if (RemainingCollectionTime > 0)
		{
			CollectingBar->SetPercent(1.0f - RemainingCollectionTime / CollectibleActor->GetCollectionTime());
		}
		else
		{
			OnFinishCollecting();
		}
	}
}
