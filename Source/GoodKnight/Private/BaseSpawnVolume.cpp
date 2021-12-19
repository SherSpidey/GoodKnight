// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseSpawnVolume.h"

#include "Components/BillboardComponent.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Player/PlayerInterface.h"

// Sets default values
ABaseSpawnVolume::ABaseSpawnVolume()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	BillboardComponent = CreateDefaultSubobject<UBillboardComponent>(TEXT("Root"));
	SetRootComponent(BillboardComponent);
 
	SpawningBox = CreateDefaultSubobject<UBoxComponent>(TEXT("SpawningBox"));
	SpawningBox->SetupAttachment(GetRootComponent());
	
}

// Called when the game starts or when spawned
void ABaseSpawnVolume::BeginPlay()
{
	Super::BeginPlay();
	SpawningBox->OnComponentBeginOverlap.AddDynamic(this, &ABaseSpawnVolume::OnOverlapBegin);
	SpawningBox->OnComponentEndOverlap.AddDynamic(this, &ABaseSpawnVolume::OnOverlapEnd);
}

// Called every frame
void ABaseSpawnVolume::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

FVector ABaseSpawnVolume::GetSpawnLocation() const
{
	return UKismetMathLibrary::RandomPointInBoundingBox(SpawningBox->GetComponentLocation(), SpawningBox->GetScaledBoxExtent());;
}

void ABaseSpawnVolume::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                      UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(OtherActor->ActorHasTag("Player"))
	{
		CanSpawn = true;
	}
}

void ABaseSpawnVolume::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if(OtherActor->ActorHasTag("Player"))
	{
		CanSpawn = false;
	}
}

void ABaseSpawnVolume::SpawnOurPawn_Implementation(UClass* ToSpawn, const FVector& Location, const FRotator& Rotation)
{
	if(ToSpawn)
	{
		UWorld* World = GetWorld();
		if(World)
		{
			float Score = 0;
			ABaseCharacter* Character = World->SpawnActor<ABaseCharacter>(ToSpawn, Location, Rotation);
			APawn* Player = UGameplayStatics::GetPlayerPawn(this, 0);
			IPlayerInterface* PlayerInterface = Cast<IPlayerInterface>(Player);
			if(PlayerInterface)
			{
				Score = PlayerInterface->Execute_GetPlayerScore(Player);
			}
			int32 LevelAdd = static_cast<int32> (Score / 100);
			if(LevelAdd + 1 >= Character->MaxLevel)
			{
				Character->Level = Character->MaxLevel;
			}
			else
			{
				Character->Level = LevelAdd + 1; 	
			}
			for(int i = 0; i < LevelAdd; i++)
			{
				Character->LevelUp();
			}
			
		}
	}
}

