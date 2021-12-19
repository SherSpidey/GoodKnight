// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/BaseWeapon.h"
#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"
#include "NiagaraComponent.h"
#include "NiagaraSystem.h"
#include "Player/PlayerInterface.h"

ABaseWeapon::ABaseWeapon()
{
	CombatBoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("CombatBox"));
	CombatBoxComponent->SetupAttachment(MainMesh);

	CombatBoxComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	CombatBoxComponent->SetCollisionObjectType(ECC_WorldStatic);
	CombatBoxComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	CombatBoxComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

	TrailComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("NiagaraTrial"));
	TrailComponent->SetupAttachment(MainMesh);
	TrailComponent->SetAutoActivate(false);
}

void ABaseWeapon::BeginPlay()
{
	Super::BeginPlay();
	CombatBoxComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	CombatBoxComponent->OnComponentBeginOverlap.AddDynamic(this, &ABaseWeapon::OnWeaponBeginOverlap);
	CombatBoxComponent->OnComponentEndOverlap.AddDynamic(this, &ABaseWeapon::OnWeaponEndOverlap);

	MainMesh->SetSimulatePhysics(true);
	if(Trail)
	{
		TrailComponent->SetAsset(Trail);
		SetTrailWidth(TrailWidth);
		SetTrailLength(TrailLength);
		//SetTrailColor(TrailColor);
	}
}

void ABaseWeapon::DisablePhysic_Implementation()
{
	MainMesh->SetSimulatePhysics(false);
}

void ABaseWeapon::Picked_Implementation()
{
	MainMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	TriggerSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ABaseWeapon::DropItem_Implementation()
{
	MainMesh->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	TriggerSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	MainMesh->SetSimulatePhysics(true);
}

void ABaseWeapon::SetDamageEnable_Implementation(bool bDamageEnable)
{
	if(bDamageEnable)
	{
		CombatBoxComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	}else
	{
		CombatBoxComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

void ABaseWeapon::OnWeaponBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                       UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(OtherActor->ActorHasTag("Enemy"))
	{
		if(Damaged)
		{
			return ;
		}
		Damaged = true;
		IPlayerInterface* PlayerInterface = Cast<IPlayerInterface>(OwnedActor);
		if(PlayerInterface)
		{
			PlayerInterface->Execute_WeaponDamageToEnemy(OwnedActor, OtherActor);
		}
	}
}

void ABaseWeapon::OnWeaponEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if(OtherActor->ActorHasTag("Enemy"))
	{
		Damaged = false;
	}
}

void ABaseWeapon::SetTrailEnable_Implementation(bool bEnable)
{
	TrailComponent->SetActive(bEnable);
}
