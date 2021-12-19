// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseCharacter.h"

#include "BaseDamageType.h"
#include "PlayerKnightController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ABaseCharacter::ABaseCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	bUseControllerRotationYaw = false;		//Character does not need to follow the controller yaw
	//Default is false, redundant 
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;

	GetMesh()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);

	//Set character facing the forward direction while moving
	GetCharacterMovement()->RotationRate = FRotator(0.f, 620.f, 0.f);
	GetCharacterMovement()->bOrientRotationToMovement = true;

	MaxHealth = 100.f;
	Health = 100.f;

	MaxLevel = 15.f;
	Level = 1.f;

}

// Called when the game starts or when spawned
void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	PlayerKnightController = Cast<APlayerKnightController>(UGameplayStatics::GetPlayerController(this, 0));

	//Some Character born with high level
	for(int i = 1; i < Level; ++i)
	{
		LevelUp();
	}
	
}

void ABaseCharacter::Death_Implementation()
{
	StopAnimMontage();
	IsAlive = false;
	if(DeathMontage)
	{
		//Set the proper death direction
		if(FVector::DotProduct(HitDirection, GetActorForwardVector()) < 0)
		{
			SetActorRotation(FRotator(0,(-HitDirection).Rotation().Yaw,0));
			PlayAnimMontage(DeathMontage, 1.f,"DeathFront");
		}
		else
		{
			SetActorRotation(FRotator(0,HitDirection.Rotation().Yaw,0));
			PlayAnimMontage(DeathMontage, 1.f,"DeathBack");
		}
	}
}


float ABaseCharacter::GetHealthPercent() const
{
	return Health / MaxHealth;
}

float ABaseCharacter::GetLevelSpeed() const
{
	return FVector(GetVelocity().X, GetVelocity().Y, 0.f).Size();
}

void ABaseCharacter::Hit_Implementation()
{
	if(!IsAlive)
	{
		UE_LOG(LogTemp, Warning, TEXT("here"));
		return ;
	}
	if(HitDamageType->HitType == EHitDamageType::EDT_Still)
	{
		return ;
	}
	//This is a example of do reaction, mostly override needed 
	// switch (HitDamageType->HitType) {
	// case EDamageType::EDT_Front:
	// 	PlayAnimMontage(HitMontage, 1, "HitFront");
	// 	break;
	// case EDamageType::EDT_Left:
	// 	PlayAnimMontage(HitMontage, 1, "HitLeft");
	// 	break;
	// case EDamageType::EDT_Right:
	// 	PlayAnimMontage(HitMontage, 1, "HitRight");
	// 	break;
	// case EDamageType::EDT_Heavy:
	// 	PlayAnimMontage(HitMontage, 1, "HitHeavy");
	// 	break;
	// case EDamageType::EDT_ToBack:
	// 	PlayAnimMontage(HitMontage, 1, "HitToBack");
	// 	break;
	// case EDamageType::EDT_Still:
	// 	//Do nothing
	// 	break;
	// default: ;
	// }
}

// Called every frame
void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABaseCharacter::LevelUp_Implementation()
{
	//Increase MaxHealth ans restore health
	MaxHealth *= 1.2;
	Health = MaxHealth;
}

void ABaseCharacter::ApplyDamage(float Damage)
{
	Health -= Damage;
	if(Health <= 0)
	{
		Health = 0;
		Death();
	}
}

void ABaseCharacter::ApplyHeal(float Heal)
{
	Health += Heal;
	if(Health > MaxHealth)
		Health = MaxHealth;
}

float ABaseCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator,
	AActor* DamageCauser)
{
	Killer = DamageCauser;
	HitDirection = (FVector(GetActorLocation().X, GetActorLocation().Y, 0) - FVector(DamageCauser->GetActorLocation().X, DamageCauser->GetActorLocation().Y, 0.f)).GetSafeNormal();
	HitDamageType = Cast<UBaseDamageType>(DamageEvent.DamageTypeClass.GetDefaultObject());
	Hit();
	ApplyDamage(DamageAmount);
	return Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
}

void ABaseCharacter::SetIsAttacking_Implementation(bool bIsAttacking)
{
	IsAttacking = bIsAttacking;
}

