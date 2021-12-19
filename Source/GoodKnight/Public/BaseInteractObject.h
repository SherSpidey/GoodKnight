// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseInteractObject.generated.h"

UCLASS()
class GOODKNIGHT_API ABaseInteractObject : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABaseInteractObject();

	//Trigger box
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Floor Switch")
	class UBoxComponent * TriggerBox;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Floor Switch")
	class UStaticMeshComponent* InteractObject;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Floor Switch")
	UStaticMeshComponent* RespondObject;

	UPROPERTY(BlueprintReadWrite, Category="Floor Switch")
	FVector InitialSwitchLocation;

	UPROPERTY(BlueprintReadWrite, Category="Floor Switch")
	FVector InitialObjectLocation;

	UPROPERTY(BlueprintReadWrite, Category="Floor Switch")
	FRotator InitialObjectRotation;
	
	UPROPERTY(BlueprintReadWrite, Category="Floor Switch")
	float SwitchTime;

	FTimerHandle SwitchHandle;

	UFUNCTION(BlueprintImplementableEvent, Category="Interact")
	void EndInteract();

	UFUNCTION(BlueprintImplementableEvent, Category="Interact")
	void BeginInteract();
	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

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
