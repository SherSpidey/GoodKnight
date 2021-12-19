// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseProjectile.h"

#include "BaseDamageType.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"

// Sets default values
ABaseProjectile::ABaseProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	NiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("NiagaraSystem"));
	SetRootComponent(NiagaraComponent);
	
	OutLine = CreateDefaultSubobject<USphereComponent>("OutLine");
	OutLine->SetupAttachment(NiagaraComponent);

	OutLine->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	OutLine->SetCollisionObjectType(ECC_WorldDynamic);
	OutLine->SetCollisionResponseToAllChannels(ECR_Overlap);

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileMovement");

	DamageNumber = 10.f;

}

// Called when the game starts or when spawned
void ABaseProjectile::BeginPlay()
{
	Super::BeginPlay();
	
	OutLine->OnComponentBeginOverlap.AddDynamic(this, &ABaseProjectile::OnProjectileBeginOverlap);
	
}

// Called every frame
void ABaseProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABaseProjectile::OnProjectileBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(ActorHasTag("EnemyProjectile"))
	{
		if(OtherActor->ActorHasTag("Player"))
		{
			if(MainDamageType)
			{
				Cast<UBaseDamageType>(MainDamageType.GetDefaultObject())->HitType = EHitDamageType::EDT_Still;
				UGameplayStatics::ApplyDamage(OtherActor, DamageNumber, nullptr, this, MainDamageType);
			}
		}
		//Prevent self destroy
		if(OtherActor->ActorHasTag("Enemy"))
		{
			return ;
		}
	}
	if(HitEmitter)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, HitEmitter, GetActorLocation(), GetActorRotation(), GetActorScale());
	}
	Destroy();
	if(ExplodeSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, ExplodeSound, GetActorLocation(), GetActorRotation());
	}
}
