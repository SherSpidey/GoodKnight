// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseEnums.h"
#include "BaseStructs.generated.h"

USTRUCT(BlueprintType)
struct FItemInfo
{
 GENERATED_BODY()
public:
 UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Item Info")
 FString ItemName;

 UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Item Info")
 EItemType ItemType;
 
};

/**
 * 
 */
class GOODKNIGHT_API BaseStructs
{
public:
};
