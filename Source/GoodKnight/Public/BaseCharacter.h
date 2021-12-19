// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacterInterface.h"
#include "GameFramework/Character.h"
#include "BaseCharacter.generated.h"

UCLASS()
class GOODKNIGHT_API ABaseCharacter : public ACharacter, public IBaseCharacterInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABaseCharacter();

	//Basic Stats Settings

	//All living creature has life and level
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Stats")
	int32 MaxLevel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Stats")
	int32 Level;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="LifeSetting")
	float MaxHealth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="LifeSetting")
	float Health;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="State")
	bool IsAlive = true;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="State")
	bool IsAttacking = false;

	UPROPERTY(BlueprintReadWrite, Category="Montage")
	bool IsAnimatingOver = true;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Montage")
	class UAnimMontage* DeathMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Montage")
	UAnimMontage* HitMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Combat")
	TSubclassOf<UDamageType> MainDamageType;

	UPROPERTY(BlueprintReadWrite, Category="Combat")
	class UBaseDamageType* HitDamageType;

	UPROPERTY(BlueprintReadOnly, Category="Hit")
	FVector HitDirection;

	UPROPERTY(BlueprintReadOnly, Category="Combat")
	AActor* Killer;

	UPROPERTY(BlueprintReadOnly, Category="Player")
	class APlayerKnightController* PlayerKnightController;
	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Stats")
	void Death();

	UFUNCTION(BlueprintPure, Category="Stats")
	float GetHealthPercent() const;

	UFUNCTION(BlueprintPure, Category="Stats")
	float GetLevelSpeed() const;
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Combat")
	void Hit();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Stats")
	void LevelUp();

	UFUNCTION(BlueprintCallable, Category="StatsUpdate")
	void ApplyDamage(float Damage);

	UFUNCTION(BlueprintCallable, Category="StatsUpdate")
	void ApplyHeal(float Heal);

	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="State")
	void SetIsAttacking(bool bIsAttacking);
	virtual void SetIsAttacking_Implementation(bool bIsAttacking) override;

	
};
