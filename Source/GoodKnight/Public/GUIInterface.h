// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "GUIInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(BlueprintType)
class UGUIInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class GOODKNIGHT_API IGUIInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void UpdateHealthBar(float InPercent);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void UpdateManaBar(float InPercent);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void UpdateStaminaBar(float InPercent);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void UpdateExpBar(float InPercent);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void UpdateLevel(int32 Level);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void UpdateScore(float Score);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void SetValue(float Value);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void SetInteract(bool bShow);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void SetScore(int32 BestScore, int32 Score);
	
};
