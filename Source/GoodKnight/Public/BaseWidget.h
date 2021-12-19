// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GUIInterface.h"
#include "Blueprint/UserWidget.h"
#include "BaseWidget.generated.h"

/**
 * 
 */
UCLASS()
class GOODKNIGHT_API UBaseWidget : public UUserWidget, public IGUIInterface
{
	GENERATED_BODY()
	
};
