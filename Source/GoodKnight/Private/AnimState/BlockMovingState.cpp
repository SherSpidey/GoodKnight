// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimState/BlockMovingState.h"

#include "Player/PlayerInterface.h"

void UBlockMovingState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);
	IPlayerInterface* PlayerInterface = Cast<IPlayerInterface>(MeshComp->GetOwner());
	if(PlayerInterface)
	{
		PlayerInterface->Execute_SetMoving(MeshComp->GetOwner(), false);
	}
}

void UBlockMovingState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::NotifyEnd(MeshComp, Animation);
	IPlayerInterface* PlayerInterface = Cast<IPlayerInterface>(MeshComp->GetOwner());
	if(PlayerInterface)
	{
		PlayerInterface->Execute_SetMoving(MeshComp->GetOwner(), true);
	}
}
