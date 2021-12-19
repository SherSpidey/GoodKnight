// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ItemInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(BlueprintType)
class UItemInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class GOODKNIGHT_API IItemInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Pickups")
	void DisablePhysic();
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Pickups")
	void Picked();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Pickups")
	void DropItem();
};
