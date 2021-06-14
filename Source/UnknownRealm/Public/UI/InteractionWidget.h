// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"

#include "InteractionWidget.generated.h"

class UProgressBar;
class ACollectibleItem;
class UTextBlock;
/**
 * 
 */
UCLASS()
class UNKNOWNREALM_API UInteractionWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UInteractionWidget(const FObjectInitializer& ObjectInitializer);

	void SetInteractText(const FString& ActionString);
	
	void OnStartCollecting(ACollectibleItem* CollectibleItem);
	void OnFinishCollecting();
	void CancelCollecting() const;

protected:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	
protected:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* InteractText;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UProgressBar* CollectingBar;

private:
	UPROPERTY()
	ACollectibleItem* CollectibleActor;

	float RemainingCollectionTime;
};
