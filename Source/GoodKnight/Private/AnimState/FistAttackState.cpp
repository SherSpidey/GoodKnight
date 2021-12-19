// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimState/FistAttackState.h"

#include "Player/PlayerInterface.h"

void UFistAttackState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);
	IPlayerInterface* PlayerInterface = Cast<IPlayerInterface>(MeshComp->GetOwner());
	if(PlayerInterface)
	{
		PlayerInterface->Execute_UpdateFistOverlapState(MeshComp->GetOwner(), true);
	}
}

void UFistAttackState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::NotifyEnd(MeshComp, Animation);
	IPlayerInterface* PlayerInterface = Cast<IPlayerInterface>(MeshComp->GetOwner());
	if(PlayerInterface)
	{
		PlayerInterface->Execute_UpdateFistOverlapState(MeshComp->GetOwner(), false);
	}
}
