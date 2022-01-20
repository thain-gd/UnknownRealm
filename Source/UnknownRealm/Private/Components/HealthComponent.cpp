#include "Components/HealthComponent.h"

#include "Combat/BaseDamageType.h"
#include "Core/MPGameInstance.h"
#include "Net/UnrealNetwork.h"
#include "Player/PlayerCharacter.h"

// Sets default values for this component's properties
UHealthComponent::UHealthComponent()
	: MaxHealth(100), CurrentHealth(MaxHealth)
{
	SetIsReplicatedByDefault(true);
}

// Called when the game starts
void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	AActor* MyOwner = GetOwner();
	if (MyOwner && MyOwner->HasAuthority())
	{
		MyOwner->OnTakeAnyDamage.AddDynamic(this, &UHealthComponent::HandleDamageTaken);
	}

	CurrentHealth = MaxHealth;
}

void UHealthComponent::HandleDamageTaken(AActor* InDamagedActor, float InDamage, const UDamageType* InDamageType, AController* InInstigatedBy, AActor* InDamageCauser)
{
	if (bIsInvincible || InDamage <= 0.0f || !IsAlive())
		return;

	const UBaseDamageType* BaseDamageType = Cast<UBaseDamageType>(InDamageType);
	if (BaseDamageType)
	{
		BaseDamageType->HandleDamage(InDamage, InDamageCauser, GetOwner());
	}
}


void UHealthComponent::TakeDamage(float InDamage, AActor* InCauser, bool bInShowDamage)
{
	CurrentHealth = FMath::Clamp(CurrentHealth - InDamage, 0.0f, MaxHealth);
	OnHealthChanged.Execute();

	if (!InCauser || !bInShowDamage)
		return;

	const APlayerCharacter* PlayerChar = Cast<APlayerCharacter>(InCauser);
	if (PlayerChar)
	{
		PlayerChar->ShowDamageDealt(InDamage);
	}
}

void UHealthComponent::SR_IncreaseHealth_Implementation(float InAmount)
{
	CurrentHealth += InAmount;
}

void UHealthComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UHealthComponent, CurrentHealth);
}
