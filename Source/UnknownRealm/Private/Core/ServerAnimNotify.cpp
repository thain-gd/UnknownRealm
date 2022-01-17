// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/ServerAnimNotify.h"

UServerAnimNotify::UServerAnimNotify()
	: WorldContext(nullptr)
{
}

void UServerAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	AActor* MeshOwner = MeshComp->GetOwner();
	if (MeshOwner && MeshOwner->HasAuthority())
	{
		UWorld* PrevWorldContext = WorldContext;
		WorldContext = MeshComp ? MeshComp->GetWorld() : nullptr;
		Received_Notify(MeshComp, Animation);
		WorldContext = PrevWorldContext;
	}
}
