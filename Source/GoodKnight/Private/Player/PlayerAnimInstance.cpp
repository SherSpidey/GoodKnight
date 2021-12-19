// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/PlayerAnimInstance.h"


void UPlayerAnimInstance::UpdatePlayerAttackMode_Implementation(EPlayerAttackMode EAttackMode)
{
	AttackMode = EAttackMode;
}

void UPlayerAnimInstance::UpdatePlayerBattleMode_Implementation(bool bIsBattleMode)
{
	IsBattleMode = bIsBattleMode;
}

void UPlayerAnimInstance::UpdateWeaponState_Implementation(bool bEquippedWeapon)
{
	EquippedWeapon = bEquippedWeapon;
}
