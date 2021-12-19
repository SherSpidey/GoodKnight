// Fill out your copyright notice in the Description page of Project Settings.


#include "Potion/BasePotion.h"
#include "Sound/SoundCue.h"
#include "Kismet/GameplayStatics.h"

void ABasePotion::BeginPlay()
{
	Super::BeginPlay();
	MainMesh->SetSimulatePhysics(true);
}

void ABasePotion::Picked_Implementation()
{
	if(PickSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, PickSound, GetActorLocation());
	}
	Destroy();
}
