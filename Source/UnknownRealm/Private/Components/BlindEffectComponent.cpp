#include "Components/BlindEffectComponent.h"

#include "Blueprint/UserWidget.h"
#include "Player/PlayerCharacter.h"

void UBlindEffectComponent::Init()
{
	APlayerCharacter* PlayerCharacter = GetOwner<APlayerCharacter>();
	check(PlayerCharacter);
	PlayerCharacter->OnPlayerDodgeRolled.Unbind();
	PlayerCharacter->OnPlayerDodgeRolled.BindUObject(this, &UBlindEffectComponent::DecreaseRemainingRollTimes);

	RemainingRollTimes = BlindRemovalRollTimes;
	GetWorld()->GetTimerManager().SetTimer(BlindRemovalTimerHandle, this, &UBlindEffectComponent::RemoveBlindEffect, LastingTime);

	if (!BlindEffectHUD)
	{
		BlindEffectHUD = CreateWidget<UUserWidget>(GetWorld(), BlindEffectHUDClass);
		BlindEffectHUD->AddToViewport();
	}
}

void UBlindEffectComponent::DecreaseRemainingRollTimes()
{
	--RemainingRollTimes;
	if (RemainingRollTimes == 0)
	{
		RemoveBlindEffect();
	}
}

void UBlindEffectComponent::RemoveBlindEffect()
{
	APlayerCharacter* PlayerCharacter = GetOwner<APlayerCharacter>();
	check(PlayerCharacter);

	BlindEffectHUD->RemoveFromParent();
	DestroyComponent();
}
