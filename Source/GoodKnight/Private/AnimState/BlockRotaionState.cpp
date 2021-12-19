// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimState/BlockRotaionState.h"
#include "GameFramework/Character.h"

void UBlockRotaionState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                     float TotalDuration)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);
	if(MeshComp->GetOwner())
	{
		if(MeshComp->GetOwner()->ActorHasTag("Player"))
		{
			ACharacter* Player = Cast<ACharacter>(MeshComp->GetOwner());
			if(Player)
			{
				Player->bUseControllerRotationYaw = false;
			}
		}
	}
}

void UBlockRotaionState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::NotifyEnd(MeshComp, Animation);
}
