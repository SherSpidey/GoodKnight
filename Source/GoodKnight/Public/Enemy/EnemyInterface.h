// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseEnums.h"
#include "UObject/Interface.h"
#include "EnemyInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UEnemyInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class GOODKNIGHT_API IEnemyInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Animation")
	void DeathEnd();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Health Bar")
	FVector GetHealBarLocation();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Movement")
	void SetMaxSpeed(float Speed);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Movement")
	bool GetNeedPatrol();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Combat")
	float Attack(EEnemyAttackMove AttackMove);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Combat")
	TArray<float> GetAttackDistances();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Combat")
	void SetWeaponDamageEnable(bool bEnable);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Combat")
	bool PeacefulState(bool bUpdate, bool NewState);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Combat")
	void SpawnProjectile();
};
