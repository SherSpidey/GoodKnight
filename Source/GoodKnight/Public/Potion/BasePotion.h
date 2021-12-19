// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseItem.h"
#include "BasePotion.generated.h"

/**
 * 
 */
UCLASS()
class GOODKNIGHT_API ABasePotion : public ABaseItem
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Sound")
	class USoundCue* PickSound;
	
	virtual void BeginPlay() override;
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Pickups")
	void Picked();
	virtual void Picked_Implementation() override;
	
};
