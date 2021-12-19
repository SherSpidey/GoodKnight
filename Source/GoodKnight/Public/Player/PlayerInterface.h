// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseEnums.h"
#include "BaseStructs.h"
#include "UObject/Interface.h"
#include "PlayerInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UPlayerInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class GOODKNIGHT_API IPlayerInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Combat State")
	EPlayerAttackMode GetAttackMode();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Combat State")
	void SetAttackMove(int32 CurrentMove);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Aniamtion")
	void SetAnimatingState(bool State);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Interact")
	void SetPickInfo(AActor* Pickup, FItemInfo ItemInfo);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Interact")
	void WeaponEquippedMode(bool bInHand);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Interact")
	void ShowMessage(bool bShow);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Movement")
	void SetMoving(bool Enable);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Stamina")
	void ReSetStaminaState();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Combat State")
	void UpdateFistOverlapState(bool bCollisionEnable);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Combat State")
	void SetWeaponDamageState(bool bDamageEnable);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Combat State")
	void WeaponDamageToEnemy(AActor* Enemy);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Death")
	void DeathEnd();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="CombatWin")
	void VictoryCheck(AActor* Enemy, float ScorePoint, float ExpPoint);
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="FX")
	void SetWeaponTrail(bool bEnable);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Score")
	float GetPlayerScore();
	
};
