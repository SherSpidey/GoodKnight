// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "BaseProjectile.h"
#include "EnemyInterface.h"
#include "EnemyCharacter.generated.h"

/**
 * 
 */
UCLASS()
class GOODKNIGHT_API AEnemyCharacter : public ABaseCharacter, public IEnemyInterface
{
	GENERATED_BODY()

	bool Damaged = false;

	AEnemyCharacter();
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Weapon")
	UStaticMeshComponent* Weapon;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Weapon")
	class UBoxComponent* WeaponBox;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Projectile")
	TSubclassOf<ABaseProjectile> Projectile;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Combat")
	TArray<float> AttackDistances;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Combat")
	bool IsBoss = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Hit")
	bool Peaceful = true;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Treasure")
	float KillPoint;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Treasure")
	float KillExp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Materail")
	UMaterialInterface* HitMaterial;

	UPROPERTY(BlueprintReadWrite, Category="Materail")
	UMaterialInterface* OriginMaterial;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Sound")
	TMap<EHitWeaponType, class USoundCue*> HitSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="DropItem")
	TSubclassOf<AActor> DropItem;
	

protected:

	virtual void Death_Implementation() override;

	virtual void Hit_Implementation() override;
	
	UPROPERTY(BlueprintReadWrite, Category="Death")
	FTimerHandle DeathTimeHandle;

	UPROPERTY(BlueprintReadWrite, Category="Health Bar")
	bool bShowHealthBar = false;

	UPROPERTY(BlueprintReadWrite, Category="Health Bar")
	FTimerHandle HealthBarHandle;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Montage")
	UAnimMontage* BattleMontage;

	UPROPERTY(BlueprintReadWrite, Category="Combat")
	EEnemyAttackMove AttackMoves = EEnemyAttackMove::EAM_Attack1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Movement")
	bool bNeedPatrol = false;

	FTimerHandle HitTimerHandle;
	
	void StopShowHealth();

	void UseNormalMaterial() const;

	void DropLoot() const;

public:
	virtual void BeginPlay() override;
	
	virtual void Tick(float DeltaSeconds) override;

	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	
	UFUNCTION(BlueprintCallable, Category="Death")
	void DestroyBody();


	UFUNCTION()
	void OnWeaponBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnWeaponEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	
	
	//Stop basic behavior
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Animation")
	void DeathEnd();
	virtual void DeathEnd_Implementation() override;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Health Bar")
	FVector GetHealBarLocation();
	virtual FVector GetHealBarLocation_Implementation() override;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Movement")
	void SetMaxSpeed(float Speed);
	virtual void SetMaxSpeed_Implementation(float Speed) override;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Movement")
	bool GetNeedPatrol();
	virtual bool GetNeedPatrol_Implementation() override;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Combat")
	float Attack(EEnemyAttackMove AttackMove);
	virtual float Attack_Implementation(EEnemyAttackMove AttackMove) override;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Combat")
	TArray<float> GetAttackDistances();
	virtual TArray<float> GetAttackDistances_Implementation() override;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Combat")
	void SetWeaponDamageEnable(bool bEnable);
	virtual void SetWeaponDamageEnable_Implementation(bool bEnable) override;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Combat")
	bool PeacefulState(bool bUpdate, bool NewState);
	virtual bool PeacefulState_Implementation(bool bUpdate, bool NewState) override;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Combat")
	void SpawnProjectile();
	virtual void SpawnProjectile_Implementation() override;
};
