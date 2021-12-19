// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "BaseStructs.h"
#include "PlayerInterface.h"
#include "PlayerCharacter.generated.h"

/**
 * 
 */


UCLASS()
class GOODKNIGHT_API APlayerCharacter : public ABaseCharacter, public IPlayerInterface
{
	GENERATED_BODY()
	//Private Stats setting
	float JogSpeed = 400.f;
	float SprintSpeed = 600.f;

	bool CanMove = true;

	bool ForwardPressed = false;
	bool BackwardPressed = false;
	bool RightPressed = false;
	bool LeftPressed = false;

	bool WeaponInHand = true;

	float StartMaxHealth;
	float StartMaxMana;
	float StartMaxStamina;
	float StartMaxExp;
	float StartMaxLevel;

	//prevent double damage
	bool Damaged = false;

	FVector TargetUILocation;

	class IAnimInterface* AnimInterface;

protected:
	//A controller reference
	//Update in HUD
	UPROPERTY(BlueprintReadOnly, Category="Controller")
	class APlayerKnightController*  HeroController;

	UPROPERTY(BlueprintReadOnly, Category="Basic")
	bool SprintEnable = false;
	
	//Player Stats properties

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Stats")
	float MaxMana;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Stats")
	float Mana;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Stats")
	float MaxStamina;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Stats")
	float Stamina;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Stats")
	float MaxExp;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Stats")
	float Exp;

	UPROPERTY(BlueprintReadWrite, Category="State")
	EPlayerStaminaState StaminaState;

	UPROPERTY(BlueprintReadWrite, Category="State")
	EPlayerAttackMode AttackMode;

	UPROPERTY(BlueprintReadWrite, Category="State")
	bool IsBattleMode = false;
	
	UPROPERTY(BlueprintReadWrite, Category="State")
    bool IsTargeting = false;
	
	FTimerHandle StaminaHandle;
	
	float ManaChangeSpeed;

	float StaminaChangeSpeed = 0.01f;

	//Montage setting
	
	UPROPERTY(BlueprintReadWrite, Category="Montage")
	int32 AttackMoveIndex = 0;
	
	UPROPERTY(EditDefaultsOnly, Category="Montage")
	TArray<UAnimMontage* > PunchMontages;

	UPROPERTY(EditDefaultsOnly, Category="Montage")
	TArray<UAnimMontage* > NoWeaponDodgeMontages;

	UPROPERTY(EditDefaultsOnly, Category="Montage")
	TArray<UAnimMontage* > KatanaMontages;

	UPROPERTY(EditDefaultsOnly, Category="Montage")
	UAnimMontage* KatanaSpecialMontages;

	UPROPERTY(EditDefaultsOnly, Category="Montage")
	TMap<EPlayerMovingDirection, UAnimMontage*> KatanaDodgeMontages;

	UPROPERTY(EditDefaultsOnly, Category="Montage")
	TArray<UAnimMontage* > BigSwordMontages;

	UPROPERTY(EditDefaultsOnly, Category="Montage")
	UAnimMontage* BigSwordSpecialMontages;

	UPROPERTY(EditDefaultsOnly, Category="Montage")
	TMap<EPlayerMovingDirection, UAnimMontage*> BigSwordDodgeMontages;

	UPROPERTY(EditDefaultsOnly, Category="Montage")
	TMap<EPlayerAttackMode, UAnimMontage*> PlayerHitMontages;

	UPROPERTY(EditDefaultsOnly, Category="Montage")
	TMap<EPlayerAttackMode, UAnimMontage*> PlayerDeathMontages;

public:

	APlayerCharacter();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

	//GameScore
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Score")
	float Score;

	//Proporties for other no mouse device
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseTurnRate;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseLookUpRate;

	UPROPERTY(BlueprintReadWrite, Category="Pickup")
	AActor* PickupItem;

	UPROPERTY(BlueprintReadWrite, Category="Pickup")
	FItemInfo PickupItemInfo;

	UPROPERTY(BlueprintReadWrite, Category="Pickup")
	AActor* EquippedWeapon;

	UPROPERTY(BlueprintReadWrite, Category="Target")
	AActor* DamagedEnemy;
	
	UPROPERTY(BlueprintReadWrite, Category="Target")
	AActor* TargetEnemy;

	UPROPERTY(BlueprintReadWrite, Category="Target")
	FTimerHandle TargetTimerHandle;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Combat")
	class USphereComponent* LeftFistSphere;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Combat")
	class USphereComponent* RightFistSphere;
	
protected:

	//Controller can only be got after gameplay
	virtual void BeginPlay() override;
	
	//Basic movement functions
	void MoveForward(float Value);

	void MoveRight(float Value);
	
	void TurnAtRate(float Rate);

	void LookUpAtRate(float Rate);

	void BaseJump();

	void BeginSprint();

	void StopSprint();

	void AddYaw(float Value);

	void AddPitch(float Value);

	void DodgeMove();

	void Interact();

	void Drop();

	//Battle state switch
	void SwitchBattleMode();

	void UpdateAttackMode(EPlayerAttackMode NewAttackMode);
	
	void UpdateAnimBattleMode() const;

	void UpdateAnimAttackMode() const;

	//Get Stats percent
	UFUNCTION(BlueprintPure, Category="Stats Percent")
	float GetManaPercent() const;

	UFUNCTION(BlueprintPure, Category="Stats Percent")
	float GetStaminaPercent() const;

	UFUNCTION(BlueprintPure, Category="Stats Percent")
	float GetExpPercent() const;

	//Attack

	//LightAttack
	void LightAttack();

	//Special Attack
	void SpecialAttack();

	//Update System
	UFUNCTION(BlueprintCallable, Category="Stats System")
	bool UpdateStamina(float Value, bool Increase);

	UFUNCTION(BlueprintCallable, Category="Stats System")
	bool UpdateMana(float Value, bool Increase);

	UFUNCTION(BlueprintCallable, Category="Stats System")
	void UpdateExp(float Value);

	UFUNCTION(BlueprintCallable, Category="Stats System")
	void UpdateHealth(float Value, bool Increase);

	//Exhausted waiting
	void ExhaustedTimer();

public:
	virtual void Tick(float DeltaSeconds) override;
	
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable, Category="Game")
	void ResetCharacter();

	UFUNCTION(BlueprintCallable, Category="State")
	bool GetIsInAir();

	UFUNCTION(BlueprintCallable, Category="State")
	EPlayerMovingDirection GetMovingDirection();

	//Target Enemy
	UFUNCTION(BlueprintCallable, Category="Targeting")
	void SetTarget();

	UFUNCTION(BlueprintCallable, Category="Targeting")
	void Targeting();

	UFUNCTION()
	void OnFistOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnFistOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	virtual void Hit_Implementation() override;

	virtual void Death_Implementation() override;

	virtual void LevelUp_Implementation() override;

	UFUNCTION(BlueprintNativeEvent, Category="Combat State")
	EPlayerAttackMode GetAttackMode();
	virtual EPlayerAttackMode GetAttackMode_Implementation() override;

	UFUNCTION(BlueprintNativeEvent, Category="Combat State")
	void SetAttackMove(int32 CurrentMove);
	virtual void SetAttackMove_Implementation(int32 CurrentMove) override;

	UFUNCTION(BlueprintNativeEvent, Category="Combat State")
	void SetAnimatingState(bool State);
	virtual void SetAnimatingState_Implementation(bool State) override;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Combat State")
	void SetPickInfo(AActor* Pickup, FItemInfo ItemInfo);
	virtual void SetPickInfo_Implementation(AActor* Pickup, FItemInfo ItemInfo) override;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Combat State")
	void WeaponEquippedMode(bool bInHand);
	virtual void WeaponEquippedMode_Implementation(bool bInHand) override;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Interact")
	void ShowMessage(bool bShow);
	virtual void ShowMessage_Implementation(bool bShow) override;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Combat State")
	void SetMoving(bool Enable);
	virtual void SetMoving_Implementation(bool Enable) override;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Combat State")
	void ReSetStaminaState();
	virtual void ReSetStaminaState_Implementation() override;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Combat State")
	void UpdateFistOverlapState(bool bCollisionEnable);
	virtual void UpdateFistOverlapState_Implementation(bool bCollisionEnable) override;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Combat State")
	void SetWeaponDamageState(bool bDamageEnable);
	virtual void SetWeaponDamageState_Implementation(bool bDamageEnable) override;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Combat State")
	void WeaponDamageToEnemy(AActor* Enemy);
	virtual void WeaponDamageToEnemy_Implementation(AActor* Enemy) override;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Death")
	void DeathEnd();
	virtual void DeathEnd_Implementation() override;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="CombatWin")
	void VictoryCheck(AActor* Enemy, float ScorePoint, float ExpPoint);
	virtual void VictoryCheck_Implementation(AActor* Enemy, float ScorePoint, float ExpPoint) override;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="FX")
	void SetWeaponTrail(bool bEnable);
	virtual void SetWeaponTrail_Implementation(bool bEnable) override;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Score")
	float GetPlayerScore();
	virtual float GetPlayerScore_Implementation() override;
};
