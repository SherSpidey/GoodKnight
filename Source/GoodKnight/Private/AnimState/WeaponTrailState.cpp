// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimState/WeaponTrailState.h"
#include "Player/PlayerInterface.h"

void UWeaponTrailState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                    float TotalDuration)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);
	if(MeshComp->GetOwner())
	{
		if(MeshComp->GetOwner()->ActorHasTag("Player"))
		{
			IPlayerInterface* PlayerInterface = Cast<IPlayerInterface>(MeshComp->GetOwner());
			if(PlayerInterface)
			{
				PlayerInterface->Execute_SetWeaponTrail(MeshComp->GetOwner(), true);
			}
		}
		if(MeshComp->GetOwner()->ActorHasTag("Enemy"))
		{
			//Enemy Weapon trail not finished
		}
	}
}

void UWeaponTrailState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::NotifyEnd(MeshComp, Animation);
	if(MeshComp->GetOwner())
	{
		if(MeshComp->GetOwner()->ActorHasTag("Player"))
		{
			IPlayerInterface* PlayerInterface = Cast<IPlayerInterface>(MeshComp->GetOwner());
			if(PlayerInterface)
			{
				PlayerInterface->Execute_SetWeaponTrail(MeshComp->GetOwner(), false);
			}
		}
		if(MeshComp->GetOwner()->ActorHasTag("Enemy"))
		{
			//Enemy Weapon trail not finished
		}
	}
}

