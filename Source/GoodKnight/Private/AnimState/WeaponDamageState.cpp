// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimState/WeaponDamageState.h"

#include "Enemy/EnemyInterface.h"
#include "Player/PlayerInterface.h"

void UWeaponDamageState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
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
				PlayerInterface->Execute_SetWeaponDamageState(MeshComp->GetOwner(), true);
			}
		}
		if(MeshComp->GetOwner()->ActorHasTag("Enemy"))
		{
			IEnemyInterface* PlayerInterface = Cast<IEnemyInterface>(MeshComp->GetOwner());
			if(PlayerInterface)
			{
				PlayerInterface->Execute_SetWeaponDamageEnable(MeshComp->GetOwner(), true);
			}
		}
	}
}

void UWeaponDamageState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::NotifyEnd(MeshComp, Animation);
	if(MeshComp->GetOwner())
	{
		if(MeshComp->GetOwner()->ActorHasTag("Player"))
		{
			IPlayerInterface* PlayerInterface = Cast<IPlayerInterface>(MeshComp->GetOwner());
			if(PlayerInterface)
			{
				PlayerInterface->Execute_SetWeaponDamageState(MeshComp->GetOwner(), false);
			}
		}
		if(MeshComp->GetOwner()->ActorHasTag("Enemy"))
		{
			IEnemyInterface* PlayerInterface = Cast<IEnemyInterface>(MeshComp->GetOwner());
			if(PlayerInterface)
			{
				PlayerInterface->Execute_SetWeaponDamageEnable(MeshComp->GetOwner(), false);
			}
		}
	}
}
