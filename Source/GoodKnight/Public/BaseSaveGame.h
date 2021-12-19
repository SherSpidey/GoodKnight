// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "BaseSaveGame.generated.h"

/**
 * 
 */
UCLASS()
class GOODKNIGHT_API UBaseSaveGame : public USaveGame
{
	GENERATED_BODY()

public:

	UBaseSaveGame();
	
	UPROPERTY(VisibleAnywhere, Category="Basic")
	FString SaveName;

	UPROPERTY(VisibleAnywhere, Category="Basic")
	uint32 UserIndex;

	UPROPERTY(VisibleAnywhere, Category="Score")
	int32 BestScore;
	
};
