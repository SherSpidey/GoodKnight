// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimState/AnimatingState.h"

#include "Player/PlayerInterface.h"

void UAnimatingState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);
	if(MeshComp->GetOwner())
	{
		if(MeshComp->GetOwner()->ActorHasTag("Player"))
		{
			IPlayerInterface* PlayerInterface = Cast<IPlayerInterface>(MeshComp->GetOwner());
			if(PlayerInterface)
			{
				PlayerInterface->Execute_SetAnimatingState(MeshComp->GetOwner(), false);
			}
		}
	}
	
}

void UAnimatingState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::NotifyEnd(MeshComp, Animation);
	if(MeshComp->GetOwner())
	{
		if(MeshComp->GetOwner()->ActorHasTag("Player"))
		{
			IPlayerInterface* PlayerInterface = Cast<IPlayerInterface>(MeshComp->GetOwner());
			if(PlayerInterface)
			{
				PlayerInterface->Execute_SetAnimatingState(MeshComp->GetOwner(), true);
			}
		}
	}
}
