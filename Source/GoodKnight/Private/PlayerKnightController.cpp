// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerKnightController.h"
#include "Blueprint/UserWidget.h"
#include "GUIInterface.h"
#include "Enemy/EnemyInterface.h"
#include "GameFramework/Character.h"
#include "Blueprint/WidgetLayoutLibrary.h"


void APlayerKnightController::BeginPlay()
{
	Super::BeginPlay();
	if(PlayerUI)
	{
		MainHUD = CreateWidget<UUserWidget>(this, PlayerUI);
		
		MainHUD->AddToViewport();
		MainHUD->SetVisibility(ESlateVisibility::Visible);
		//Default set
		ResetStats();
	}
}


void APlayerKnightController::ResetStats()
{
	UpdateHealth(1.f);
	UpdateMana(1.f);
	UpdateStamina(1.f);
	UpdateExp(0.f);
	UpdateLevel(1);
	UpdateScore(0.f);
}

//Maybe you should to delay before call this function
void APlayerKnightController::UpdateHealth(float NewHealthPercent)
{
	if(MainHUD)
	{
		//Must create a UMG class from cpp or this cast will fail
		IGUIInterface* MessageBroader = Cast<IGUIInterface>(MainHUD);
		if(MessageBroader)
		{
			MessageBroader->Execute_UpdateHealthBar(MainHUD, NewHealthPercent);
		}
	}
}

void APlayerKnightController::UpdateMana(float NewManaPercent)
{
	if(MainHUD)
	{
		//Must create a UMG class from cpp or this cast will fail
		IGUIInterface* MessageBroader = Cast<IGUIInterface>(MainHUD);
		if(MessageBroader)
		{
			MessageBroader->Execute_UpdateManaBar(MainHUD, NewManaPercent);
		}
	}
}

void APlayerKnightController::UpdateStamina(float NewStaminaPercent)
{
	if(MainHUD)
	{
		//Must create a UMG class from cpp or this cast will fail
		IGUIInterface* MessageBroader = Cast<IGUIInterface>(MainHUD);
		if(MessageBroader)
		{
			MessageBroader->Execute_UpdateStaminaBar(MainHUD, NewStaminaPercent);
		}
	}
}

void APlayerKnightController::UpdateExp(float NewExpPercent)
{
	if(MainHUD)
	{
		//Must create a UMG class from cpp or this cast will fail
		IGUIInterface* MessageBroader = Cast<IGUIInterface>(MainHUD);
		if(MessageBroader)
		{
			MessageBroader->Execute_UpdateExpBar(MainHUD, NewExpPercent);
		}
	}
}

void APlayerKnightController::UpdateLevel(int32 Level)
{
	if(MainHUD)
	{
		//Must create a UMG class from cpp or this cast will fail
		IGUIInterface* MessageBroader = Cast<IGUIInterface>(MainHUD);
		if(MessageBroader)
		{
			MessageBroader->Execute_UpdateLevel(MainHUD, Level);
		}
	}
}

void APlayerKnightController::UpdateScore(float Score)
{
	if(MainHUD)
	{
		//Must create a UMG class from cpp or this cast will fail
		IGUIInterface* MessageBroader = Cast<IGUIInterface>(MainHUD);
		if(MessageBroader)
		{
			MessageBroader->Execute_UpdateScore(MainHUD, Score);
		}
	}
}

void APlayerKnightController::UpdateInteractMessage(bool bShown)
{
	if(MainHUD)
	{
		//Must create a UMG class from cpp or this cast will fail
		IGUIInterface* MessageBroader = Cast<IGUIInterface>(MainHUD);
		if(MessageBroader)
		{
			MessageBroader->Execute_SetInteract(MainHUD, bShown);
		}
	}
}

void APlayerKnightController::ShowLevelUp(FVector Location)
{
	UUserWidget* LevelUpTemp = CreateWidget<UUserWidget>(this, LevelUpWidget);
	LevelUpTemp->SetVisibility(ESlateVisibility::Visible);
	LevelUpTemp->AddToViewport();
	LevelUpTemp->SetAlignmentInViewport(FVector2D(.5f, 0.f));
	FVector2D PositionInViewport;
	ProjectWorldLocationToScreen(Location, PositionInViewport);
	LevelUpTemp->SetPositionInViewport(PositionInViewport);
	// const FVector2D Size(400.f, 400.f);
	// LevelUpTemp->SetDesiredSizeInViewport(Size);
}

void APlayerKnightController::ShowDamageValue_Implementation(FVector Location, float Value)
{
	UUserWidget* TempDamage = CreateWidget<UUserWidget>(this, DamageWidget);
	IGUIInterface* MessageBroader = Cast<IGUIInterface>(TempDamage);
	if(MessageBroader)
	{
		MessageBroader->Execute_SetValue(TempDamage, Value);
	}
	TempDamage->SetVisibility(ESlateVisibility::Visible);
	TempDamage->AddToViewport();
	TempDamage->SetAlignmentInViewport(FVector2D(.5f, 0.f));
	FVector2D PositionInViewport;
	ProjectWorldLocationToScreen(Location, PositionInViewport);
	TempDamage->SetPositionInViewport(PositionInViewport);
}

void APlayerKnightController::PlayerDeath(int32 BestScore, int32 Score)
{
	UWidgetLayoutLibrary::RemoveAllWidgets(this);
	DeathWidget = CreateWidget<UUserWidget>(this, DeathUI);
	DeathWidget->AddToViewport();
	DeathWidget->SetVisibility(ESlateVisibility::Visible);

	IGUIInterface* MessageBroader = Cast<IGUIInterface>(DeathWidget);
	if(MessageBroader)
	{
		MessageBroader->Execute_SetScore(DeathWidget, BestScore, Score);
	}
	
}

void APlayerKnightController::ReStartGame()
{
	UWidgetLayoutLibrary::RemoveAllWidgets(this);
	MainHUD = CreateWidget<UUserWidget>(this, PlayerUI);
	MainHUD->AddToViewport();
	MainHUD->SetVisibility(ESlateVisibility::Visible);
	//Default set
	ResetStats();
}

void APlayerKnightController::SetTargetingState(FVector Location, bool bShow)
{
	if(TargetUI == nullptr)
	{
		
		TargetUI = CreateWidget<UUserWidget>(this, TargetWidget);
		TargetUI->AddToViewport();
		TargetUI->SetVisibility(ESlateVisibility::Visible);
		TargetUI->SetAlignmentInViewport(FVector2D(.5f, 0.f));
		const FVector2D Size(40.f, 40.f);
		TargetUI->SetDesiredSizeInViewport(Size);
	}
	if(TargetUI)
	{
		if(bShow)
		{
			TargetUI->SetVisibility(ESlateVisibility::Visible);
			FVector2D PositionInViewport;
			ProjectWorldLocationToScreen(Location, PositionInViewport);
			TargetUI->SetPositionInViewport(PositionInViewport);
		}
		else
		{
			TargetUI->SetVisibility(ESlateVisibility::Hidden);
		}
	}
}

void APlayerKnightController::EnemyBarUpdate_Implementation(ACharacter* Enemy, float Percent, int32 Level, bool bDestroy, bool bIsBoss)
{
	if(Enemy == nullptr)
		return;
	if(bDestroy)
	{
		//Already removed
		if(EnemyHealthBars.Find(Enemy) == nullptr)
		{
			return ;
		}
		if(EnemyHealthBars[Enemy])
		{
			EnemyHealthBars[Enemy]->RemoveFromViewport();
			EnemyHealthBars.Remove(Enemy);
		}
	}else
	{
		if(EnemyHealthBars.Find(Enemy) == nullptr)
		{
			EnemyHealthBars.Add(Enemy, CreateWidget<UUserWidget>(this, EnemyUI));
			EnemyHealthBars[Enemy]->AddToViewport();
			EnemyHealthBars[Enemy]->SetVisibility(ESlateVisibility::Visible);
			IGUIInterface* MessageBroader = Cast<IGUIInterface>(EnemyHealthBars[Enemy]);
			if(MessageBroader)
			{
				MessageBroader->Execute_UpdateLevel(EnemyHealthBars[Enemy], Level);
			}
			
		}
		if(EnemyHealthBars[Enemy])
		{
			if(bIsBoss)
			{
				EnemyHealthBars[Enemy]->SetAlignmentInViewport(FVector2D(.5f, 0.f));
				FVector2D PositionInViewport;
				const FVector2D BarSize(400.f, 35.f);
				FVector HealthBarLocation(0.f, 0.f, 0.f);
			
				IEnemyInterface* EnemyInterface = Cast<IEnemyInterface>(Enemy);
				if(EnemyInterface)
				{
					HealthBarLocation = EnemyInterface->Execute_GetHealBarLocation(Enemy);
				}
				ProjectWorldLocationToScreen(HealthBarLocation, PositionInViewport);
				EnemyHealthBars[Enemy]->SetPositionInViewport(PositionInViewport);
				EnemyHealthBars[Enemy]->SetDesiredSizeInViewport(BarSize);
			}else
			{
				EnemyHealthBars[Enemy]->SetAlignmentInViewport(FVector2D(.5f, 0.f));
				FVector2D PositionInViewport;
				const FVector2D BarSize(250.f, 25.f);
				FVector HealthBarLocation(0.f, 0.f, 0.f);
			
				IEnemyInterface* EnemyInterface = Cast<IEnemyInterface>(Enemy);
				if(EnemyInterface)
				{
					HealthBarLocation = EnemyInterface->Execute_GetHealBarLocation(Enemy);
				}
				ProjectWorldLocationToScreen(HealthBarLocation, PositionInViewport);
				EnemyHealthBars[Enemy]->SetPositionInViewport(PositionInViewport);
				EnemyHealthBars[Enemy]->SetDesiredSizeInViewport(BarSize);
			}
			
			IGUIInterface* MessageBroader = Cast<IGUIInterface>(EnemyHealthBars[Enemy]);
			if(MessageBroader)
			{
				MessageBroader->Execute_UpdateHealthBar(EnemyHealthBars[Enemy], Percent);
			}
		}
	}
}
