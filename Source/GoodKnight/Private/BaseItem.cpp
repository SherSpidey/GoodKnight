// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseItem.h"

#include "Components/SphereComponent.h"
#include "GameFramework/Character.h"
#include "Player/PlayerInterface.h"

// Sets default values
ABaseItem::ABaseItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MainMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemMesh"));
	SetRootComponent(MainMesh);

	TriggerSphere = CreateDefaultSubobject<USphereComponent>(TEXT("CoverSphere"));
	TriggerSphere->SetupAttachment(GetRootComponent());

	MainMesh->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	MainMesh->SetCollisionObjectType(ECC_WorldDynamic);
	MainMesh->SetCollisionResponseToAllChannels(ECR_Ignore);
	MainMesh->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Block);
	MainMesh->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);

	TriggerSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	TriggerSphere->SetCollisionObjectType(ECC_WorldStatic);
	TriggerSphere->SetCollisionResponseToAllChannels(ECR_Ignore);
	TriggerSphere->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

	TriggerSphere->SetSphereRadius(40.f);
	
}

// Called when the game starts or when spawned
void ABaseItem::BeginPlay()
{
	Super::BeginPlay();
	TriggerSphere->OnComponentBeginOverlap.AddDynamic(this, &ABaseItem::OnOverlapBegin);
	TriggerSphere->OnComponentEndOverlap.AddDynamic(this, &ABaseItem::OnOverlapEnd);
	
}

void ABaseItem::SetPickupInfoToPlayer(AActor* PlayerCharacter, bool bSend)
{
	IPlayerInterface* PlayerInterface = Cast<IPlayerInterface>(PlayerCharacter);
	if(PlayerInterface)
	{
		if(bSend)
		{
			PlayerInterface->Execute_SetPickInfo(PlayerCharacter, this, ItemInfo);
			PlayerInterface->Execute_ShowMessage(PlayerCharacter, true); 
		}
		else
		{
			PlayerInterface->Execute_SetPickInfo(PlayerCharacter, nullptr, ItemInfo);
			PlayerInterface->Execute_ShowMessage(PlayerCharacter, false);
		}
	}
}

// Called every frame
void ABaseItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABaseItem::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(OtherActor->ActorHasTag("Player"))
	{
		SetPickupInfoToPlayer(OtherActor, true);
		OwnedActor = OtherActor;
	}
}

void ABaseItem::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if(OtherActor->ActorHasTag("Player"))
	{
		SetPickupInfoToPlayer(OtherActor, false);
	}
}

