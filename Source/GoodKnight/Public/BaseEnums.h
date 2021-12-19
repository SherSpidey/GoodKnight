// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
/**
 * 
 */
UENUM(BlueprintType)
enum class EPlayerStatsType:uint8
{
 EST_Health UMETA(DisplayName = "Heath"),
 EST_Mana UMETA(DisplayName = "Mana"),
 EST_Stamina UMETA(DisplayName = "Stamina"),
 EST_Exp UMETA(DisplayName = "Exp"),
};

UENUM(BlueprintType)
enum class EPlayerStaminaState:uint8
{
 ESS_Recover UMETA(DisplayName = "Recover"),
 ESS_Run UMETA(DisplayName = "Run"),
 ESS_Defence UMETA(DisplayName = "Defence"),
 ESS_Blank UMETA(DisplayName = "Blank"),
 ESS_Exhausted UMETA(DisplayName = "Exhausted"),
};

//how the stamina decrease
UENUM(BlueprintType)
enum class EPlayerMovementState:uint8
{
 EMS_Idle UMETA(DisplayName = "Idle"),
 EMS_Jog UMETA(DisplayName = "Jog"),
 EMS_Run UMETA(DisplayName = "Run"),
 EMS_Attack UMETA(DisplayName = "Attack"),
};

//Attack types
UENUM(BlueprintType)
enum class EPlayerAttackMode:uint8
{
 EAM_NoWeapon UMETA(DisplayName = "NoWeapon"),
 EAM_BigSword UMETA(DisplayName = "BigSword"),
 EAM_Katana UMETA(DisplayName = "Katana")
};

//Directions while moving
UENUM(BlueprintType)
enum class EPlayerMovingDirection:uint8
{
 EMD_Idle UMETA(DisplayName = "Idle"),
 EMD_Forward UMETA(DisplayName = "Forward"),
 EMD_BackWard UMETA(DisplayName = "BackWard"),
 EMD_Right UMETA(DisplayName = "Right"),
 EMD_Left UMETA(DisplayName = "Left"),
 EMD_ForRight UMETA(DisplayName = "ForRight"),
 EMD_ForLeft UMETA(DisplayName = "ForLeft"),
 EMD_BackRight UMETA(DisplayName = "BackRight"),
 EMD_BackLedt UMETA(DisplayName = "BackLeft"),
};

//Item types
UENUM(BlueprintType)
enum class EItemType:uint8
{
 EIT_Katana UMETA(DisplayName = "Katana"),
 EIT_BigSword UMETA(DisplayName = "BigSword"),
 EIT_PotionRed UMETA(DisplayName = "RedPotion"),
 EIT_PotionBlue UMETA(DisplayName = "BluePotion")
};

//Damage hit type
UENUM(BlueprintType)
enum class EHitDamageType:uint8
{
 EDT_Front UMETA(DisplayName = "Hit From Front"),
 EDT_Left UMETA(DisplayName = "Hit From Left"),
 EDT_Right UMETA(DisplayName = "Hit From Right"),
 EDT_Heavy UMETA(DisplayName = "Hit Heavy"),
 EDT_ToBack UMETA(DisplayName = "Hit back"),
 
 EDT_Still UMETA(DisplayName = "No Reaction")
};

//Damage hit type
UENUM(BlueprintType)
enum class EHitWeaponType:uint8
{
 EWT_Sword UMETA(DisplayName = "Sword"),
 EWT_Katana UMETA(DisplayName = "Katana"),
 
 EWT_Fist UMETA(DisplayName = "Fist")
};

UENUM(BlueprintType)
enum class EEnemyAttackMove:uint8
{
 EAM_Attack1 UMETA(DisplayName = "Attack1"),
 EAM_Attack2 UMETA(DisplayName = "Attack2"),
 EAM_Heavy UMETA(DisplayName = "Heavy"),
 EAM_Range UMETA(DisplayName = "Range")
};

class GOODKNIGHT_API BaseEnums
{
};
