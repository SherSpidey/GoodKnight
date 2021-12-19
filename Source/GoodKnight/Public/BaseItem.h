// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseStructs.h"
#include "ItemInterface.h"
#include "GameFramework/Actor.h"
#include "BaseItem.generated.h"

UCLASS()
class GOODKNIGHT_API ABaseItem : public AActor, public IItemInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABaseItem();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Mesh")
	class UStaticMeshComponent* MainMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Overlap")
	class USphereComponent* TriggerSphere;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Info")
	FItemInfo ItemInfo;

	UPROPERTY(BlueprintReadOnly, Category="Character")
	AActor* OwnedActor;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	UFUNCTION(BlueprintCallable, Category="PickUp")
	void SetPickupInfoToPlayer(AActor* PlayerCharacter, bool bSend);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

};
