// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseProjectile.generated.h"

UCLASS()
class GOODKNIGHT_API ABaseProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABaseProjectile();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Basic")
	class UNiagaraComponent* NiagaraComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Basic")
	class UNiagaraSystem* HitEmitter;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Basic")
	class USphereComponent* OutLine;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Baisc")
	class UProjectileMovementComponent* ProjectileMovementComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Baisc")
	float DamageNumber;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Baisc")
	class USoundCue* ExplodeSound;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Combat")
	TSubclassOf<UDamageType> MainDamageType;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void OnProjectileBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	// UFUNCTION()
	// void OnProjectileEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	// 	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

};
