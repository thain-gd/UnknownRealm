// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "NotifierWidget.generated.h"

class UCanvasPanel;
/**
 *		A widget that handles all notifications for gameplays including showing dealing damage
 */
UCLASS()
class UNKNOWNREALM_API UNotifierWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintImplementableEvent)
	void ShowDamage(int32 Damage, bool bIsCrit);

protected:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UCanvasPanel* MainPanel;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> FloatingDamageWidgetClass;
};
