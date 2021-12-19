// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "PlayerKnightController.generated.h"

/**
 * 
 */
UCLASS()
class GOODKNIGHT_API APlayerKnightController : public APlayerController
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="HUD")
	TSubclassOf<UUserWidget> PlayerUI;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="HUD")
	TSubclassOf<UUserWidget> DamageWidget;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="HUD")
	TSubclassOf<UUserWidget> LevelUpWidget;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="HUD")
	TSubclassOf<UUserWidget> TargetWidget;

	UPROPERTY(BlueprintReadOnly, Category="HUD")
	class UUserWidget* TargetUI;

	UPROPERTY(BlueprintReadOnly, Category="HUD")
	UUserWidget* MainHUD;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="HUD")
	TSubclassOf<UUserWidget> EnemyUI;

	UPROPERTY(BlueprintReadOnly, Category="HUD")
	TMap<ACharacter*, UUserWidget*> EnemyHealthBars;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="HUD")
	TSubclassOf<UUserWidget> DeathUI;

	UPROPERTY(BlueprintReadOnly, Category="HUD")
	UUserWidget* DeathWidget;

public:
	UFUNCTION(BlueprintCallable, Category="Update")
	void ResetStats();
	
	UFUNCTION(BlueprintCallable, Category="Update")
	void UpdateHealth(float NewHealthPercent);

	UFUNCTION(BlueprintCallable, Category="Update")
	void UpdateMana(float NewManaPercent);

	UFUNCTION(BlueprintCallable, Category="Update")
	void UpdateStamina(float NewStaminaPercent);

	UFUNCTION(BlueprintCallable, Category="Update")
	void UpdateExp(float NewExpPercent);

	UFUNCTION(BlueprintCallable, Category="Update")
	void UpdateLevel(int32 Level);

	UFUNCTION(BlueprintCallable, Category="Update")
	void UpdateScore(float Score);

	UFUNCTION(BlueprintCallable, Category="Update")
	void UpdateInteractMessage(bool bShow);

	UFUNCTION(BlueprintCallable, Category="Update")
	void ShowLevelUp(FVector Location);
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="EnemyUpdate")
	void EnemyBarUpdate(ACharacter* Enemy, float Percent, int32 Level,  bool bDestroy, bool bIsBoss);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Damage")
	void ShowDamageValue(FVector Location, float Value);

	UFUNCTION(BlueprintCallable, Category="Death")
	void PlayerDeath(int32 BestScore, int32 Score);
	
	UFUNCTION(BlueprintCallable, Category="Death")
	void ReStartGame();

	UFUNCTION(BlueprintCallable, Category="Target")
	void SetTargetingState(FVector Location, bool bShow);
	
};
