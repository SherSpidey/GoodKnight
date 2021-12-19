// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseEnums.h"
#include "GameFramework/DamageType.h"
#include "BaseDamageType.generated.h"

/**
 * 
 */
UCLASS()
class GOODKNIGHT_API UBaseDamageType : public UDamageType
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Hit Reaction")
	EHitDamageType HitType;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Hit Weapon")
	EHitWeaponType HitWeapon;
	
};
