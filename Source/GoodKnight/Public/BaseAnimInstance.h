// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "BaseAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class GOODKNIGHT_API UBaseAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
protected:
	
	virtual void NativeInitializeAnimation() override;

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	
	UPROPERTY(BlueprintReadOnly, Category="AnimState")
	bool IsInAir;

	UPROPERTY(BlueprintReadOnly, Category="AnimState")
	bool IsMoving;

	UPROPERTY(BlueprintReadOnly, Category="AnimState")
	float Speed;

	UPROPERTY(BlueprintReadOnly, Category="AnimState")
	float Direction;

	UPROPERTY(BlueprintReadOnly, Category="AnimState")
	APawn* Character;

	UFUNCTION(BlueprintCallable, Category="AnimaUpdate")
	void UpdateMovementState();
	
};
