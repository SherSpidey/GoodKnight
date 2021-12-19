// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AnimInterface.h"
#include "BaseAnimInstance.h"
#include "PlayerAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class GOODKNIGHT_API UPlayerAnimInstance : public UBaseAnimInstance, public IAnimInterface
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadWrite, Category="LegState")
	bool IsLeft = true;

	UPROPERTY(BlueprintReadWrite, Category="Attack Mode")
	EPlayerAttackMode AttackMode;

	UPROPERTY(BlueprintReadWrite, Category="Attack Mode")
	bool IsBattleMode;

	UPROPERTY(BlueprintReadWrite, Category="Attack Mode")
	bool EquippedWeapon = false;

public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="AnimUpdate")
	void UpdatePlayerAttackMode(EPlayerAttackMode EAttackMode);
	virtual void UpdatePlayerAttackMode_Implementation(EPlayerAttackMode EAttackMode) override;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="AnimUpdate")
	void UpdatePlayerBattleMode(bool bIsBattleMode);
	virtual void UpdatePlayerBattleMode_Implementation(bool bIsBattleMode) override;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="AnimUpdate")
	void UpdateWeaponState(bool bEquippedWeapon);
	virtual void UpdateWeaponState_Implementation(bool bEquippedWeapon) override;
	
};
