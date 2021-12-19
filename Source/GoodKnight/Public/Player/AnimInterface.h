// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseEnums.h"
#include "UObject/Interface.h"
#include "AnimInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UAnimInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class GOODKNIGHT_API IAnimInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="AnimUpdate")
	void UpdatePlayerAttackMode(EPlayerAttackMode EAttackMode);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="AnimUpdate")
	void UpdatePlayerBattleMode(bool bIsBattleMode);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="AnimUpdate")
	void UpdateWeaponState(bool bEquippedWeapon);
};
