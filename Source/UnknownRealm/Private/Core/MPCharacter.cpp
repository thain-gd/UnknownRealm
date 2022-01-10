#include "Core/MPCharacter.h"

#include "GameFramework/CharacterMovementComponent.h"

void AMPCharacter::MC_PlayMontage_Implementation(UAnimMontage* InMontage)
{
	PlayAnimMontage(InMontage);
}

void AMPCharacter::MC_PauseMontage_Implementation(const UAnimMontage* InMontage) const
{
	GetAnimInstance()->Montage_Pause(InMontage);
}

void AMPCharacter::MC_ResumeMontage_Implementation(const UAnimMontage* InMontage) const
{
	GetAnimInstance()->Montage_Resume(InMontage);
}

void AMPCharacter::MC_StopMontage_Implementation(const UAnimMontage* InMontage) const
{
	GetAnimInstance()->Montage_Stop(0.5f, InMontage);
}

bool AMPCharacter::IsMontagePlaying(const UAnimMontage* InMontage) const
{
	return GetAnimInstance()->Montage_IsPlaying(InMontage);
}

bool AMPCharacter::IsAnyMontagePlaying() const
{
	return GetAnimInstance()->IsAnyMontagePlaying();
}

bool AMPCharacter::CheckMontageSection(const FName& InSection) const
{
	return GetAnimInstance()->Montage_GetCurrentSection() == InSection;
}

void AMPCharacter::SR_SetMovementSpeed_Implementation(const float NewSpeed) const
{
	MC_SetMovementSpeed(NewSpeed);
}

void AMPCharacter::MC_SetMovementSpeed_Implementation(const float NewSpeed) const
{
	GetCharacterMovement()->MaxWalkSpeed = NewSpeed;
}

UAnimInstance* AMPCharacter::GetAnimInstance() const
{
	return GetMesh()->GetAnimInstance();
}
