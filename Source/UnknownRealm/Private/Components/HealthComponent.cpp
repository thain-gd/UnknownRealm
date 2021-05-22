#include "Components/HealthComponent.h"

// Sets default values for this component's properties
UHealthComponent::UHealthComponent()
	: MaxHealth(100), CurrentHealth(MaxHealth)
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
}


// Called when the game starts
void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	if (GetOwnerRole() == ROLE_Authority)
	{
		AActor* MyOwner = GetOwner();
		if (MyOwner)
			MyOwner->OnTakeAnyDamage.AddDynamic(this, &UHealthComponent::HandleDamageTaken);
	}

	CurrentHealth = MaxHealth;
}

void UHealthComponent::HandleDamageTaken(AActor* OnTakeAnyDamage, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	if (Damage <= 0.0f || !IsAlive())
		return;

	CurrentHealth = FMath::Clamp(CurrentHealth - (int32)Damage, 0, MaxHealth);
	OnHealthChanged.Execute();
}
