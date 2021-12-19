// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseItem.h"
#include "WeaponInterface.h"
#include "BaseWeapon.generated.h"

/**
 * 
 */
UCLASS()
class GOODKNIGHT_API ABaseWeapon : public ABaseItem, public IWeaponInterface
{
	GENERATED_BODY()

	bool Damaged = false;

public:
	ABaseWeapon();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Combat")
	class UBoxComponent* CombatBoxComponent;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="FX")
	class UNiagaraComponent* TrailComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="FX")
	class UNiagaraSystem* Trail;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="FX")
	float TrailWidth;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="FX")
	float TrailLength;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="FX")
	FLinearColor TrailColor;
	
protected:
	
	UFUNCTION(BlueprintImplementableEvent, Category="FX")
	void SetTrailWidth(float Width);

	UFUNCTION(BlueprintImplementableEvent, Category="FX")
	void SetTrailLength(float Length);

	UFUNCTION(BlueprintImplementableEvent, Category="FX")
	void SetTrailColor(FLinearColor Color);

public:
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnWeaponBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnWeaponEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Pickups")
	void DisablePhysic();
	virtual void DisablePhysic_Implementation() override;
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Pickups")
	void Picked();
	virtual void Picked_Implementation() override;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Pickups")
	void DropItem();
	virtual void DropItem_Implementation() override;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Combat")
	void SetDamageEnable(bool bDamageEnable);
	virtual void SetDamageEnable_Implementation(bool bDamageEnable) override;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="FX")
	void SetTrailEnable(bool bEnable);
	virtual void SetTrailEnable_Implementation(bool bEnable) override;
};
