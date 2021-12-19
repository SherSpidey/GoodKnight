// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/EnemyCharacter.h"
#include "BaseDamageType.h"
#include "BaseEnums.h"
#include "PlayerKnightController.h"
#include "Components/ArrowComponent.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Player/PlayerInterface.h"
#include "Sound/SoundCue.h"

AEnemyCharacter::AEnemyCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	Weapon = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Weapon"));
	Weapon->SetupAttachment(GetMesh());
	Weapon->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	WeaponBox = CreateDefaultSubobject<UBoxComponent>(TEXT("WeaponBox"));
	WeaponBox->SetupAttachment(Weapon);

	WeaponBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	WeaponBox->SetCollisionObjectType(ECC_WorldStatic);
	WeaponBox->SetCollisionResponseToAllChannels(ECR_Ignore);
	WeaponBox->SetCollisionResponseToChannel(ECC_Pawn,ECR_Overlap);

	KillPoint = 10.f;
	KillExp = 120.f;
	
	Tags.Add("Enemy");
}

void AEnemyCharacter::Death_Implementation()
{
	Super::Death_Implementation();
	//Dead enemy can not be treated as enemy
	if(Killer)
	{
		IPlayerInterface* PlayerInterface = Cast<IPlayerInterface>(Killer);
		if(PlayerInterface)
		{
			PlayerInterface->Execute_VictoryCheck(Killer, this, KillPoint * (1 + log(Level)), KillExp * (1 + log(Level)));
		}
	}
	DropLoot();
	Tags.Remove("Enemy");
	
}

void AEnemyCharacter::Hit_Implementation()
{
	if(!IsAlive)
	{
		return ;
	}
	//Play Hit Sound
	if(HitSound.Num() != 0)
	{
		if(HitSound.Contains(HitDamageType->HitWeapon))
		{
			UGameplayStatics::PlaySoundAtLocation(this, HitSound[HitDamageType->HitWeapon], GetActorLocation());
		}
	}
	//Still State mean you can not affect enemy with current attack
	if(HitDamageType->HitType == EHitDamageType::EDT_Still)
	{
		return ;
	}
	if(HitMontage)
	{
		//Set the proper hit direction
		if(FVector::DotProduct(HitDirection, GetActorForwardVector()) < 0)
		{
			SetActorRotation(FRotator(0,(-HitDirection).Rotation().Yaw,0));
			PlayAnimMontage(HitMontage, 1.f,"HitFront");
		}
		else
		{
			SetActorRotation(FRotator(0,HitDirection.Rotation().Yaw,0));
			PlayAnimMontage(HitMontage, 1.f,"HitBack");
		}
	}
	switch (HitDamageType->HitType) {
	case EHitDamageType::EDT_Heavy:
		StopAnimMontage();
		GetCharacterMovement()->AddImpulse(HitDirection * 2000.f, true);
		break;
	case EHitDamageType::EDT_ToBack:
		StopAnimMontage();
		GetCharacterMovement()->AddImpulse(HitDirection * 3000.f, true);
		break;
	default: ;
	}
}

void AEnemyCharacter::StopShowHealth()
{
	bShowHealthBar = false;
	PlayerKnightController->EnemyBarUpdate(this, GetHealthPercent(), Level, true, IsBoss);
}

void AEnemyCharacter::UseNormalMaterial() const
{
	if(OriginMaterial)
	{
		GetMesh()->SetMaterial(0, OriginMaterial);
	}
}

void AEnemyCharacter::DropLoot() const
{
	UWorld* WorldTemp = GetWorld();
	if(WorldTemp)
	{
		if(DropItem)
		{
			WorldTemp->SpawnActor<AActor>(DropItem, GetActorLocation(), GetActorRotation());
		}
	}
}

void AEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();
	WeaponBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	WeaponBox->OnComponentBeginOverlap.AddDynamic(this, &AEnemyCharacter::OnWeaponBeginOverlap);
	WeaponBox->OnComponentEndOverlap.AddDynamic(this, &AEnemyCharacter::OnWeaponEndOverlap);

	OriginMaterial = GetMesh()->GetMaterials()[0];
}

void AEnemyCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	if(bShowHealthBar)
	{
		PlayerKnightController->EnemyBarUpdate(this, GetHealthPercent(), Level, false, IsBoss);
	}
}

float AEnemyCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator,
	AActor* DamageCauser)
{
	Peaceful = false;
	bShowHealthBar = true;
	PlayerKnightController->ShowDamageValue(GetMesh()->GetBoneLocation("Pelvis"), DamageAmount);
	if(HitMaterial)
	{
		GetMesh()->SetMaterial(0, HitMaterial);
	}
	GetWorldTimerManager().ClearTimer(HealthBarHandle);
	GetWorldTimerManager().SetTimer(HealthBarHandle, this, &AEnemyCharacter::StopShowHealth, 20.f);
	GetWorldTimerManager().ClearTimer(HitTimerHandle);
	GetWorldTimerManager().SetTimer(HitTimerHandle, this, &AEnemyCharacter::UseNormalMaterial, 0.06f);
	return Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
}

void AEnemyCharacter::DestroyBody()
{
	PlayerKnightController->EnemyBarUpdate(this, GetHealthPercent(), Level, true, IsBoss);
	Destroy();
}

void AEnemyCharacter::OnWeaponBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(OtherActor->ActorHasTag("Player"))
	{
		if(Damaged)
		{
			return ;
		}
		Damaged = true;
		switch (AttackMoves) {
		case EEnemyAttackMove::EAM_Attack1:
			Cast<UBaseDamageType>(MainDamageType.GetDefaultObject())->HitType = EHitDamageType::EDT_Right;
			UGameplayStatics::ApplyDamage(OtherActor,10.f, GetController(), this, MainDamageType);
			break;
		case EEnemyAttackMove::EAM_Attack2:
			Cast<UBaseDamageType>(MainDamageType.GetDefaultObject())->HitType = EHitDamageType::EDT_Right;
			UGameplayStatics::ApplyDamage(OtherActor,10.f, GetController(), this, MainDamageType);
			break;
		case EEnemyAttackMove::EAM_Heavy:
			Cast<UBaseDamageType>(MainDamageType.GetDefaultObject())->HitType = EHitDamageType::EDT_Heavy;
			UGameplayStatics::ApplyDamage(OtherActor,30.f, GetController(), this, MainDamageType);
			break;
		default: ;
		}
	}
}

void AEnemyCharacter::OnWeaponEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if(OtherActor->ActorHasTag("Player"))
	{
		Damaged = false;
	}
}

void AEnemyCharacter::DeathEnd_Implementation()
{
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetMesh()->SetSimulatePhysics(true);
	GetMesh()->SetCollisionProfileName(FName("Ragdoll"));

	//After 5 sec destroy the body
	GetWorldTimerManager().SetTimer(DeathTimeHandle, this, &AEnemyCharacter::DestroyBody, 5.f);
}

FVector AEnemyCharacter::GetHealBarLocation_Implementation()
{
	return GetMesh()->GetSocketLocation("HealthBar");
}

void AEnemyCharacter::SetMaxSpeed_Implementation(float Speed)
{
	GetCharacterMovement()->MaxWalkSpeed = Speed;
}

bool AEnemyCharacter::GetNeedPatrol_Implementation()
{
	return bNeedPatrol;
}

float AEnemyCharacter::Attack_Implementation(EEnemyAttackMove AttackMove)
{
	if(!IsAlive)
	{
		return 0.0f;
	}
	AttackMoves = AttackMove;
	float PlayTime = 0.0f;
	if(IsAnimatingOver)
	{
		StopAnimMontage();
		if(BattleMontage)
		{
			switch (AttackMove) {
			case EEnemyAttackMove::EAM_Attack1:
				PlayAnimMontage(BattleMontage, 1.f,"Attack1");
				PlayTime = BattleMontage->GetSectionLength(0);
				break;
			case EEnemyAttackMove::EAM_Attack2:
				PlayAnimMontage(BattleMontage, 1.f,"Attack2");
				PlayTime = BattleMontage->GetSectionLength(1);
				break;
			case EEnemyAttackMove::EAM_Heavy:
				PlayAnimMontage(BattleMontage, 1.f,"Heavy");
				PlayTime = BattleMontage->GetSectionLength(2);
				break;
			case EEnemyAttackMove::EAM_Range:
				PlayAnimMontage(BattleMontage, 1.f,"Range");
				PlayTime = BattleMontage->GetSectionLength(3);
				break;
			default: ;
			}
		}
	}
	return PlayTime;
}

TArray<float> AEnemyCharacter::GetAttackDistances_Implementation()
{
	return AttackDistances;
}

void AEnemyCharacter::SetWeaponDamageEnable_Implementation(bool bEnable)
{
	if(bEnable)
	{
		WeaponBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	}else
	{
		WeaponBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

bool AEnemyCharacter::PeacefulState_Implementation(bool bUpdate, bool bNewState)
{
	if(bUpdate)
	{
		Peaceful = bNewState;
	}
	return Peaceful;
}

void AEnemyCharacter::SpawnProjectile_Implementation()
{
	UWorld* WorldTemp = GetWorld();
	if(WorldTemp)
	{
		if(Projectile)
		{
			WorldTemp->SpawnActor<ABaseProjectile>(Projectile,GetMesh()->GetSocketLocation("Projectile"), FRotator(GetActorRotation().Pitch + 5.f, GetActorRotation().Yaw, 0));
		}
	}
}
