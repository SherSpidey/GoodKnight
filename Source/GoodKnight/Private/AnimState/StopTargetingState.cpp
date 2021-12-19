// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimState/StopTargetingState.h"

#include "Player/PlayerCharacter.h"

void UStopTargetingState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                      float TotalDuration)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);
	if(MeshComp->GetOwner())
	{
		if(MeshComp->GetOwner()->ActorHasTag("Player"))
		{
			APlayerCharacter* TempCharacter = Cast<APlayerCharacter>(MeshComp->GetOwner());
			if(TempCharacter)
			{
				if(TempCharacter->TargetEnemy)
				{
					Target = TempCharacter->TargetEnemy;
					TempCharacter->TargetEnemy = nullptr;
				}
			}
		}
	}
}

void UStopTargetingState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::NotifyEnd(MeshComp, Animation);
	if(MeshComp->GetOwner())
	{
		if(MeshComp->GetOwner()->ActorHasTag("Player"))
		{
			APlayerCharacter* TempCharacter = Cast<APlayerCharacter>(MeshComp->GetOwner());
			if(TempCharacter)
			{
				if(Target)
				{
					TempCharacter->TargetEnemy = Target;
				}
			}
		}
	}
}
