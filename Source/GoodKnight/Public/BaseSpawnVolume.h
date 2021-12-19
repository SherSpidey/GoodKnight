// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "GameFramework/Actor.h"
#include "BaseSpawnVolume.generated.h"

UCLASS()
class GOODKNIGHT_API ABaseSpawnVolume : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABaseSpawnVolume();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Spwaning")
	class UBoxComponent* SpawningBox;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Spwaning")
	class UBillboardComponent* BillboardComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Spawning")
	TSubclassOf<ABaseCharacter> PawnToSpawn;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Spwaning")
	bool CanSpawn;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintPure, Category="Spawning")
	FVector GetSpawnLocation() const;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Spawning")
	void SpawnOurPawn(UClass* ToSpawn, const FVector& Location, const FRotator& Rotation);

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

};
