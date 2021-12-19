// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/PlayerCharacter.h"
#include "BaseDamageType.h"
#include "ItemInterface.h"
#include "PlayerKnightController.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Player/AnimInterface.h"
#include "BaseSaveGame.h"
#include "Weapon/WeaponInterface.h"

APlayerCharacter::APlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	// Create a spring for camera
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(GetRootComponent());
	CameraBoom->TargetArmLength = 350.f;
	CameraBoom->SetRelativeLocation(FVector(0.f, 0.f, 60.f));
	CameraBoom->bUsePawnControlRotation = true;	//Camera follows the player controller.

	//Create camera on the spring
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;	//Spring already follows the controller

	//The size remain the default
	GetCapsuleComponent()->SetCapsuleSize(34.f, 88.f);

	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;		//Can apply movement offset when in air

	GetCharacterMovement()->MaxWalkSpeed = JogSpeed;

	LeftFistSphere = CreateDefaultSubobject<USphereComponent>(TEXT("LeftFist"));
	LeftFistSphere->SetupAttachment(GetMesh(), "LeftFist");
	LeftFistSphere->SetSphereRadius(15.f);
	LeftFistSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	LeftFistSphere->SetCollisionObjectType(ECC_WorldStatic);
	LeftFistSphere->SetCollisionResponseToAllChannels(ECR_Ignore);
	LeftFistSphere->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	
	RightFistSphere = CreateDefaultSubobject<USphereComponent>(TEXT("RightFist"));
	RightFistSphere->SetupAttachment(GetMesh(), "RightFist");
	RightFistSphere->SetSphereRadius(15.f);
	RightFistSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	RightFistSphere->SetCollisionObjectType(ECC_WorldStatic);
	RightFistSphere->SetCollisionResponseToAllChannels(ECR_Ignore);
	RightFistSphere->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	
	//A comfortable setting
	BaseTurnRate = 65.f;
	BaseLookUpRate = 65.f;

	//Initial State Setting
	StartMaxHealth = MaxHealth;
	MaxMana = 100.f;
	StartMaxMana = MaxMana;
	MaxStamina = 100.f;
	StartMaxStamina = MaxStamina;
	MaxExp = 200.f;
	StartMaxExp = MaxExp;
	StartMaxLevel = MaxLevel;

	StaminaState = EPlayerStaminaState::ESS_Recover;

	AttackMode = EPlayerAttackMode::EAM_NoWeapon;

	Tags.Add("Player");
	
}

void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	//Get the controller
	HeroController = Cast<APlayerKnightController>(GetController());
	AnimInterface = Cast<IAnimInterface>(GetMesh()->GetAnimInstance());

	if(HeroController)
	{
		HeroController->UpdateHealth(GetHealthPercent());
		HeroController->UpdateMana(GetManaPercent());
		HeroController->UpdateExp(GetExpPercent());
		HeroController->UpdateStamina(GetStaminaPercent());
		HeroController->UpdateLevel(Level);
		HeroController->UpdateScore(Score);
	}

	LeftFistSphere->OnComponentBeginOverlap.AddDynamic(this, &APlayerCharacter::OnFistOverlapBegin);
	LeftFistSphere->OnComponentEndOverlap.AddDynamic(this, &APlayerCharacter::OnFistOverlapEnd);
	RightFistSphere->OnComponentBeginOverlap.AddDynamic(this, &APlayerCharacter::OnFistOverlapBegin);
	RightFistSphere->OnComponentEndOverlap.AddDynamic(this, &APlayerCharacter::OnFistOverlapEnd);

	LeftFistSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	RightFistSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	//ResetCharacter();
}

void APlayerCharacter::MoveForward(float Value)
{
	if(!IsAlive)
	{
		return;
	}
	if(!CanMove)
	{
		return ;
	}
	if(Controller != nullptr && Value != 0.f)
	{
		if(Value > 0)
		{
			ForwardPressed = true;
			BackwardPressed = false;
		}else
		{
			BackwardPressed = true;
			ForwardPressed = false;
		}
		/**
		 *Get the Yaw and make a new rotator
		 *Use the new rotator as to get the forward vector
		 */
		if(AttackMode == EPlayerAttackMode::EAM_NoWeapon || !IsBattleMode)
		{
			AddMovementInput(FRotationMatrix(
									   FRotator(0.f, Controller->GetControlRotation().Yaw, 0.f))
									   .GetUnitAxis(EAxis::X), Value);
		}else
		{
			AddMovementInput(GetActorForwardVector(), Value);
		}
		//You can only use controller yaw when in combat and have no target
		if(AttackMode != EPlayerAttackMode::EAM_NoWeapon && IsBattleMode && !TargetEnemy)
		{
			//GetController()->SetControlRotation(GetActorRotation());
			bUseControllerRotationYaw = true;
		}else
		{
			bUseControllerRotationYaw = false;
		}
	}else
	{
		ForwardPressed = false;
		BackwardPressed = false;
		bUseControllerRotationYaw = false;
	}

}

void APlayerCharacter::MoveRight(float Value)
{
	if(!IsAlive)
	{
		return;
	}
	if(!CanMove)
	{
		return ;
	}
	if(Controller != nullptr && Value != 0.f)
	{
		if(Value > 0)
		{
			RightPressed = true;
			LeftPressed = false;
		}else
		{
			LeftPressed = true;
			RightPressed = false;
		}
		/**
		 *Get the Yaw and make a new rotator
		 *Use the new rotator as to get the right vector
		 */
		if(AttackMode == EPlayerAttackMode::EAM_NoWeapon || !IsBattleMode)
		{
			AddMovementInput(FRotationMatrix(
										FRotator(0.f, Controller->GetControlRotation().Yaw, 0.f))
										.GetUnitAxis(EAxis::Y), Value);
		}else
		{
			AddMovementInput(GetActorRightVector(), Value);
		}
	}else
	{
		LeftPressed = false;
		RightPressed = false;
	}
}

void APlayerCharacter::TurnAtRate(float Rate)
{
	if(!IsAlive)
	{
		return;
	}
	if(Controller != nullptr)
	{
		AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->DeltaTimeSeconds);
	}
}

void APlayerCharacter::LookUpAtRate(float Rate)
{
	if(!IsAlive)
	{
		return;
	}
	if(Controller != nullptr)
	{
		AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->DeltaTimeSeconds);
	}
}

void APlayerCharacter::BaseJump()
{
	if(!IsAlive)
	{
		return;
	}
	if(!IsAnimatingOver)
	{
		return ;
	}
	ACharacter::Jump();
}

void APlayerCharacter::BeginSprint()
{
	if(!IsAlive)
	{
		return;
	}
	SprintEnable = true;
	GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;	
}

void APlayerCharacter::StopSprint()
{
	if(!IsAlive)
	{
		return;
	}
	if(StaminaState != EPlayerStaminaState::ESS_Exhausted)
	{
		StaminaState = EPlayerStaminaState::ESS_Recover;
	}
	SprintEnable = false;
	GetCharacterMovement()->MaxWalkSpeed = JogSpeed;
}

void APlayerCharacter::AddYaw(float Value)
{
	if(!IsAlive)
	{
		return;
	}
	APawn::AddControllerYawInput(Value);
}

void APlayerCharacter::AddPitch(float Value)
{
	if(!IsAlive)
	{
		return;
	}
	APawn::AddControllerPitchInput(Value);
}

void APlayerCharacter::DodgeMove()
{
	if(!IsAlive)
	{
		return;
	}
	const EPlayerMovingDirection Direction = GetMovingDirection();
	if(IsAnimatingOver && !GetIsInAir())
	{
		StopAnimMontage();
		if(AttackMode != EPlayerAttackMode::EAM_NoWeapon && IsBattleMode)
		{
			switch (AttackMode) {
			case EPlayerAttackMode::EAM_BigSword:
				//Hard code force get four dodge montage
				if(BigSwordDodgeMontages.Num() != 4)
				{
					return;
				}
				if(Direction == EPlayerMovingDirection::EMD_Forward ||
						Direction == EPlayerMovingDirection::EMD_BackWard ||
						Direction == EPlayerMovingDirection::EMD_Left ||
						Direction == EPlayerMovingDirection::EMD_Right ||
						Direction == EPlayerMovingDirection::EMD_Idle)
				{
					if(UpdateStamina(15.f, false))
					{
						if(Direction == EPlayerMovingDirection::EMD_Idle)
						{
							PlayAnimMontage(BigSwordDodgeMontages[EPlayerMovingDirection::EMD_BackWard]);
						}
						else
						{
							PlayAnimMontage(BigSwordDodgeMontages[Direction]);
						}
					}
				}
				break;
			case EPlayerAttackMode::EAM_Katana:
				//Hard code force get four dodge montage
				if(KatanaDodgeMontages.Num() != 4)
				{
					return;
				}
				if(Direction == EPlayerMovingDirection::EMD_Forward ||
						Direction == EPlayerMovingDirection::EMD_BackWard ||
						Direction == EPlayerMovingDirection::EMD_Left ||
						Direction == EPlayerMovingDirection::EMD_Right ||
						Direction == EPlayerMovingDirection::EMD_Idle)
				{
					if(UpdateStamina(15.f, false))
					{
						if(Direction == EPlayerMovingDirection::EMD_Idle)
						{
							PlayAnimMontage(KatanaDodgeMontages[EPlayerMovingDirection::EMD_BackWard]);
						}
						else
						{
							PlayAnimMontage(KatanaDodgeMontages[Direction]);
						}
					}
				}
				break;
			default: ;
			}
		}
		else	//Play No weapon dodge montage
		{
			if(UpdateStamina(15.f, false))
			{
				if(NoWeaponDodgeMontages.Num() < 2)
				{
					return;
				}
				if(Direction == EPlayerMovingDirection::EMD_Idle)
				{
					PlayAnimMontage(NoWeaponDodgeMontages[0]);
				}else
				{
					PlayAnimMontage(NoWeaponDodgeMontages[1]);
				}
			}
		}
	}
}

void APlayerCharacter::Interact()
{
	if(!IsAlive)
	{
		return;
	}
	if(PickupItem)
	{
		IItemInterface* PickUpInterface = Cast<IItemInterface>(PickupItem);
		if(PickUpInterface)
		{
			switch (PickupItemInfo.ItemType)
			{
				case EItemType::EIT_Katana:
					if(EquippedWeapon)
					{
						return;
					}
					PickUpInterface->Execute_DisablePhysic(PickupItem);
					PickupItem->AttachToComponent(GetMesh(),  FAttachmentTransformRules::SnapToTargetNotIncludingScale, "Katana");
					EquippedWeapon = PickupItem;
					UpdateAttackMode(EPlayerAttackMode::EAM_Katana);
					break;
				case EItemType::EIT_BigSword:
					if(EquippedWeapon)
					{
						return;
					}
					PickUpInterface->Execute_DisablePhysic(PickupItem);
					PickupItem->AttachToComponent(GetMesh(),  FAttachmentTransformRules::SnapToTargetNotIncludingScale, "BigSword");
					EquippedWeapon = PickupItem;
					UpdateAttackMode(EPlayerAttackMode::EAM_BigSword);
					break;
				case EItemType::EIT_PotionRed:
					UpdateHealth(20, true);
					break;
				case EItemType::EIT_PotionBlue:
					UpdateMana(30, true);
					break;
				default: ;
			}
			PickUpInterface->Execute_Picked(PickupItem);
			PickupItem = nullptr;
		}
	}
}

void APlayerCharacter::Drop()
{
	if(EquippedWeapon)
	{
		EquippedWeapon->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		IItemInterface* PickUpInterface = Cast<IItemInterface>(EquippedWeapon);
		if(PickUpInterface)
		{
			PickUpInterface->Execute_DropItem(EquippedWeapon);
		}
		EquippedWeapon = nullptr;

		//Update to animation
		if(AnimInterface)
		{
			AnimInterface->Execute_UpdateWeaponState(GetMesh()->GetAnimInstance(), false);
		}
	}
	UpdateAttackMode(EPlayerAttackMode::EAM_NoWeapon);
}

void APlayerCharacter::SwitchBattleMode()
{
	IsBattleMode = !IsBattleMode;
	if(AttackMode == EPlayerAttackMode::EAM_BigSword)
	{
		//Big Sword only has battle mod
		IsBattleMode = true;
	}
	UpdateAnimBattleMode();
	if(AttackMode != EPlayerAttackMode::EAM_NoWeapon)
	{
		GetCharacterMovement()->bOrientRotationToMovement = !IsBattleMode;
		switch (AttackMode) {
			case EPlayerAttackMode::EAM_BigSword:
				JogSpeed = 200.f;
				SprintSpeed = 400.f;
				StopSprint();
				break;
			case EPlayerAttackMode::EAM_Katana:
				if(IsBattleMode)
				{
					JogSpeed = 180.f;
					SprintSpeed = 270.f;
					StopSprint();
				}else
				{
					JogSpeed = 400.f;
					SprintSpeed = 600.f;
					StopSprint();
				}
				break;
			default: ;
		}
	}else
	{
		JogSpeed = 400.f;
		SprintSpeed = 600.f;
		StopSprint();
		GetCharacterMovement()->bOrientRotationToMovement = true;
	}
}

void APlayerCharacter::UpdateAttackMode(EPlayerAttackMode NewAttackMode)
{
	AttackMode = NewAttackMode;
	if(IsBattleMode)
	{
		SwitchBattleMode();
	}
	UpdateAnimAttackMode();
}

void APlayerCharacter::UpdateAnimBattleMode() const
{
	if(AnimInterface)
	{
		AnimInterface->Execute_UpdatePlayerBattleMode(GetMesh()->GetAnimInstance(), IsBattleMode);
	}
}

void APlayerCharacter::UpdateAnimAttackMode() const
{
	if(AnimInterface)
	{
		AnimInterface->Execute_UpdatePlayerAttackMode(GetMesh()->GetAnimInstance(), AttackMode);
	}
}

float APlayerCharacter::GetManaPercent() const
{
	return Mana / MaxMana;
}

float APlayerCharacter::GetStaminaPercent() const
{
	return Stamina / MaxStamina;
}

float APlayerCharacter::GetExpPercent() const
{
	return Exp / MaxExp;
}

void APlayerCharacter::LightAttack()
{
	if(!IsAlive)
	{
		return;
	}
	if(IsBattleMode)
	{
		//katana mode may can not attack when hide weapon
		if(AttackMode == EPlayerAttackMode::EAM_Katana && !WeaponInHand)
		{
			return ;
		}
		if(IsAnimatingOver){
			if(AttackMoveIndex < 0)
			{
				AttackMoveIndex = 0;
			}
			switch (AttackMode)
			{
				case EPlayerAttackMode::EAM_NoWeapon:
					if(UpdateStamina(2, false))
					{
						StopAnimMontage();
						PlayAnimMontage(PunchMontages[AttackMoveIndex]);
						AttackMoveIndex++;
						if(AttackMoveIndex >= PunchMontages.Num())
						{
							AttackMoveIndex = 0;
						}
						//Stop stamina recovery
						StaminaState = EPlayerStaminaState::ESS_Blank;
					}
					break;
				case EPlayerAttackMode::EAM_BigSword:
					if(UpdateStamina(14, false))
					{
						StopAnimMontage();
						PlayAnimMontage(BigSwordMontages[AttackMoveIndex]);
						AttackMoveIndex++;
						if(AttackMoveIndex >= BigSwordMontages.Num())
						{
							AttackMoveIndex = 0;
						}
						//Stop increase stamina
						StaminaState = EPlayerStaminaState::ESS_Blank;
					}
					break;
				case EPlayerAttackMode::EAM_Katana: 
					if(UpdateStamina(8, false))
					{
						StopAnimMontage();
						PlayAnimMontage(KatanaMontages[AttackMoveIndex]);
						AttackMoveIndex++;
						if(AttackMoveIndex >= KatanaMontages.Num())
						{
							AttackMoveIndex = 0;
						}
						//Stop increase stamina
						StaminaState = EPlayerStaminaState::ESS_Blank;
					}
					break;
				default: ;
			}
		}
	}
	else
	{
		SwitchBattleMode();
	}
}

void APlayerCharacter::SpecialAttack()
{
	if(!IsAlive)
	{
		return;
	}
	if(IsBattleMode)
	{
		if(AttackMode == EPlayerAttackMode::EAM_Katana && !WeaponInHand)
		{
			return ;
		}
		if(IsAnimatingOver){
			switch (AttackMode)
			{
			case EPlayerAttackMode::EAM_BigSword:
				if(UpdateMana(15.f, false))
				{
					AttackMoveIndex = -1;
					StopAnimMontage();
					if(BigSwordSpecialMontages)
					{
						PlayAnimMontage(BigSwordSpecialMontages);
					}
				}
				break;
			case EPlayerAttackMode::EAM_Katana:
				if(UpdateMana(10.f, false))
				{
					AttackMoveIndex = -1;
					StopAnimMontage();
					if(KatanaSpecialMontages)
					{
						PlayAnimMontage(KatanaSpecialMontages);
					}
				}
				break;
			default: ;
			}
		}
	}
	else
	{
		SwitchBattleMode();
	}
}

void APlayerCharacter::SetTarget()
{
	if(AttackMode == EPlayerAttackMode::EAM_NoWeapon || !IsBattleMode)
	{
		IsTargeting = false;
		TargetEnemy = nullptr;
		//NoWeapon or not battle mode can not targeting
		return;
	}
	//Cancle Target if Targeting
	if(TargetEnemy)
	{
		IsTargeting = false;
		TargetUILocation = FVector(0,0,0);
		HeroController->SetTargetingState(TargetUILocation, false);
		TargetEnemy = nullptr;
		GetWorldTimerManager().ClearTimer(TargetTimerHandle);
		return ;
	}
	//Set Target is target exist
	if(DamagedEnemy)
	{
		TargetEnemy = DamagedEnemy;
		IsTargeting = true;
		GetWorldTimerManager().ClearTimer(TargetTimerHandle);
		GetWorldTimerManager().SetTimer(TargetTimerHandle, this ,&APlayerCharacter::Targeting, 0.0001, true);
	}
}

void APlayerCharacter::Targeting()
{
	if(TargetEnemy)
	{
		const FVector Direction = (TargetEnemy->GetActorLocation() - GetActorLocation());
		const FRotator TargetRotation = FMath::RInterpTo(GetActorRotation(), FRotator(0,Direction.Rotation().Yaw,0), 0.0001, 6);
		SetActorRotation(TargetRotation);
	}
	if(IsTargeting)
	{
		if(TargetEnemy)
		{
			ACharacter* EnemyTemp = Cast<ACharacter>(TargetEnemy);
			if(EnemyTemp)
			{
				TargetUILocation = EnemyTemp->GetMesh()->GetBoneLocation("spine_03");
			}
		}else
		{
			ACharacter* EnemyTemp = Cast<ACharacter>(DamagedEnemy);
			if(EnemyTemp)
			{
				TargetUILocation = EnemyTemp->GetMesh()->GetBoneLocation("spine_03");
			}
		}
		HeroController->SetTargetingState(TargetUILocation, true);
	}
}

bool APlayerCharacter::UpdateStamina(float Value, bool Increase)
{
	if(Increase)
	{
		Stamina += Value;
	}
	else
	{
		//Current move is not affordable
		if(Stamina - Value < 0.f)
		{
			return false;
		}
		Stamina -= Value;
	}
	if(Stamina > MaxStamina)
	{
		Stamina = MaxStamina;
	}
	if(Stamina < 0.f)
	{
		Stamina = 0.f;
	}
	HeroController->UpdateStamina(GetStaminaPercent());
	return true;
}

bool APlayerCharacter::UpdateMana(float Value, bool Increase)
{
	if(Increase)
	{
		Mana += Value;
	}
	else
	{
		//Current move is not affordable
		if(Mana - Value < 0.f)
		{
			return false;
		}
		Mana -= Value;
	}
	if(Mana > MaxMana)
	{
		Mana = MaxMana;
	}
	if(Mana < 0.f)
	{
		Mana = 0.f;
	}
	HeroController->UpdateMana(GetManaPercent());
	return true;
}
void APlayerCharacter::UpdateHealth(float Value, bool Increase)
{
	if(Increase)
	{
		Health += Value;
	}
	else
	{
		Health -= Value;
	}
	if(Health > MaxHealth)
	{
		Health = MaxHealth;
	}
	if(Health < 0.f)
	{
		Health = 0.f;
	}
	HeroController->UpdateHealth(GetHealthPercent());
}

void APlayerCharacter::UpdateExp(float Value)
{
	if(Level == MaxLevel)
	{
		Exp = 0.f;
	}
	else
	{
		Exp += Value;
		if(Exp >= MaxExp)
		{
			Exp -= MaxExp;
			LevelUp();
			UpdateExp(0);
		}
	}
	HeroController->UpdateExp(GetExpPercent());
}

void APlayerCharacter::ExhaustedTimer()
{
	StaminaState = EPlayerStaminaState::ESS_Recover;
}

//UpdateState
void APlayerCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	
	if(!IsAlive)
	{
		return;
	}
	
	if(SprintEnable && GetLevelSpeed())
	{
		StaminaState = EPlayerStaminaState::ESS_Run;
		StaminaChangeSpeed = 0.1f;
	}else if(!SprintEnable && GetLevelSpeed())
	{
		StaminaChangeSpeed = 0.05f;
	}else if(!GetLevelSpeed())
	{
		StaminaChangeSpeed = 0.1f;
	}

	//Update Stamina
	switch (StaminaState)
	{
		case EPlayerStaminaState::ESS_Recover:
			if(Stamina < MaxStamina)
			{
				UpdateStamina(StaminaChangeSpeed, true);
			}
			break;
		case EPlayerStaminaState::ESS_Run:
			if(!UpdateStamina(StaminaChangeSpeed, false))
			{
				StopSprint();
				StaminaState = EPlayerStaminaState::ESS_Exhausted;
			}
			break;
		case EPlayerStaminaState::ESS_Defence:
			break;
		case EPlayerStaminaState::ESS_Blank:
			//Do nothing
			break;
		case EPlayerStaminaState::ESS_Exhausted:
			if(!GetWorldTimerManager().TimerExists(StaminaHandle))
			{
				GetWorldTimerManager().SetTimer(StaminaHandle, this, &APlayerCharacter::ExhaustedTimer, 2);
			}
			break;
		default: ;
	}
	
}

void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	check(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this , &APlayerCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &APlayerCharacter::MoveRight);
	PlayerInputComponent->BindAxis("Turn", this, &APlayerCharacter::AddYaw);
	PlayerInputComponent->BindAxis("LookUp", this, &APlayerCharacter::AddPitch);
	PlayerInputComponent->BindAxis("TurnRate", this, &APlayerCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUpRate", this, &APlayerCharacter::LookUpAtRate);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &APlayerCharacter::BaseJump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &APlayerCharacter::BeginSprint);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &APlayerCharacter::StopSprint);
	PlayerInputComponent->BindAction("SwitchBattleMode", IE_Pressed, this, &APlayerCharacter::SwitchBattleMode);
	PlayerInputComponent->BindAction("LightAttack", IE_Pressed, this, &APlayerCharacter::LightAttack);
	PlayerInputComponent->BindAction("Dodge", IE_Pressed, this, &APlayerCharacter::DodgeMove);
	PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &APlayerCharacter::Interact);
	PlayerInputComponent->BindAction("SpecialAttack", IE_Pressed, this, &APlayerCharacter::SpecialAttack);
	PlayerInputComponent->BindAction("Drop", IE_Pressed, this, &APlayerCharacter::Drop);
	PlayerInputComponent->BindAction("Target", IE_Pressed, this, &APlayerCharacter::SetTarget);
}

void APlayerCharacter::ResetCharacter()
{
	IsAlive = true;
	//Initial State Setting
	MaxHealth = StartMaxHealth;
	Health = MaxHealth;
	MaxLevel = StartMaxLevel;
	Level = 1;
	MaxMana = StartMaxMana;
	Mana = MaxMana;
	MaxStamina = StartMaxStamina;
	Stamina = MaxStamina;
	MaxExp = StartMaxExp;
	
	Exp = 0.f;
	Score = 0.f;

	UpdateAnimAttackMode();
	UpdateAnimBattleMode();
	
	if(HeroController)
	{
		HeroController->RestartLevel();
		HeroController->ReStartGame();
	}
}

bool APlayerCharacter::GetIsInAir()
{
	return GetCharacterMovement()->IsFalling();
}

EPlayerMovingDirection APlayerCharacter::GetMovingDirection()
{
	if(ForwardPressed && RightPressed)
	{
		return  EPlayerMovingDirection::EMD_ForRight;
	}
	if(ForwardPressed && LeftPressed)
	{
		return EPlayerMovingDirection::EMD_ForLeft;
	}
	if(BackwardPressed && RightPressed)
	{
		return EPlayerMovingDirection::EMD_BackRight;
	}
	if(BackwardPressed && LeftPressed)
	{
		return EPlayerMovingDirection::EMD_BackLedt;
	}
	if(ForwardPressed)
	{
		return EPlayerMovingDirection::EMD_Forward;
	}
	if(RightPressed)
	{
		return EPlayerMovingDirection::EMD_Right;
	}
	if(LeftPressed)
	{
		return EPlayerMovingDirection::EMD_Left;
	}
	if(BackwardPressed)
	{
		return EPlayerMovingDirection::EMD_BackWard;
	}
	return EPlayerMovingDirection::EMD_Idle;
}

void APlayerCharacter::OnFistOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(OtherActor->ActorHasTag("Enemy"))
	{
		//Prevent double damage
		if(Damaged)
		{
			return ;	
		}
		DamagedEnemy = OtherActor;
		Damaged = true;
		Cast<UBaseDamageType>(MainDamageType.GetDefaultObject())->HitType = EHitDamageType::EDT_Still;
		Cast<UBaseDamageType>(MainDamageType.GetDefaultObject())->HitWeapon = EHitWeaponType::EWT_Fist;
		UGameplayStatics::ApplyDamage(OtherActor,5.f, GetController(), this, MainDamageType);
	}
}

void APlayerCharacter::OnFistOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if(OtherActor->ActorHasTag("Enemy"))
	{
		Damaged = false;
	}
}

float APlayerCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator,
	AActor* DamageCauser)
{
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	if(HeroController)
	{
		HeroController->UpdateHealth(GetHealthPercent());
	}
	return 0.f;
}

EPlayerAttackMode APlayerCharacter::GetAttackMode_Implementation()
{
	return  AttackMode;
}

void APlayerCharacter::SetAttackMove_Implementation(int32 CurrentMove)
{
	AttackMoveIndex = CurrentMove;
}
void APlayerCharacter::SetAnimatingState_Implementation(bool State)
{
	IsAnimatingOver = State;
}

void APlayerCharacter::SetPickInfo_Implementation(AActor* Pickup, FItemInfo ItemInfo)
{
	PickupItem = Pickup;
	PickupItemInfo = ItemInfo;
}

void APlayerCharacter::WeaponEquippedMode_Implementation(bool bInHand)
{
	if(EquippedWeapon)
	{
		if(bInHand)
		{
			switch (AttackMode) {
				case EPlayerAttackMode::EAM_NoWeapon: break;
				case EPlayerAttackMode::EAM_BigSword: break;
				case EPlayerAttackMode::EAM_Katana:
					EquippedWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, "Katana");
					WeaponInHand = true;
					break;
				default: ;
			}
		}else
		{
			switch (AttackMode) {
			case EPlayerAttackMode::EAM_NoWeapon: break;
			case EPlayerAttackMode::EAM_BigSword: break;
			case EPlayerAttackMode::EAM_Katana:
				EquippedWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, "KatanaHide");
				WeaponInHand = false;
				break;
			default: ;
			}
		}
	}
}

void APlayerCharacter::ShowMessage_Implementation(bool bShow)
{
	if(HeroController)
	{
		//Once equipped, stop showing
		if(EquippedWeapon && bShow)
		{
			if(PickupItem)
			{
				if((PickupItemInfo.ItemType == EItemType::EIT_Katana || PickupItemInfo.ItemType == EItemType::EIT_BigSword))
				{
					return ;
				}
			}
		}
		HeroController->UpdateInteractMessage(bShow);
		
	}
}

void APlayerCharacter::SetMoving_Implementation(bool Enable)
{
	CanMove = Enable;
}

void APlayerCharacter::ReSetStaminaState_Implementation()
{
	StaminaState = EPlayerStaminaState::ESS_Recover;
}

void APlayerCharacter::UpdateFistOverlapState_Implementation(bool bCollisionEnable)
{
	if(bCollisionEnable)
	{
		//Attack Index == 1, right punch
		if(!AttackMoveIndex)
		{
			LeftFistSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		}else
		{
			RightFistSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		}
	}else
	{
		LeftFistSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		RightFistSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

void APlayerCharacter::SetWeaponDamageState_Implementation(bool bDamageEnable)
{
	IWeaponInterface* WeaponInterface = Cast<IWeaponInterface>(EquippedWeapon);
	if(WeaponInterface)
	{
		WeaponInterface->Execute_SetDamageEnable(EquippedWeapon, bDamageEnable);
	}
}

void APlayerCharacter::WeaponDamageToEnemy_Implementation(AActor* Enemy)
{
	DamagedEnemy = Enemy;
	switch (AttackMode) {
	case EPlayerAttackMode::EAM_BigSword:
		Cast<UBaseDamageType>(MainDamageType.GetDefaultObject())->HitWeapon = EHitWeaponType::EWT_Sword;
		switch (AttackMoveIndex)
		{
		case -1:
			Cast<UBaseDamageType>(MainDamageType.GetDefaultObject())->HitType = EHitDamageType::EDT_ToBack;
			UGameplayStatics::ApplyDamage(Enemy,40.f, GetController(), this, MainDamageType);
			break;
		case 1:
			Cast<UBaseDamageType>(MainDamageType.GetDefaultObject())->HitType = EHitDamageType::EDT_Right;
			UGameplayStatics::ApplyDamage(Enemy,15.f, GetController(), this, MainDamageType);
			break;
		case 2:
			Cast<UBaseDamageType>(MainDamageType.GetDefaultObject())->HitType = EHitDamageType::EDT_Right;
			UGameplayStatics::ApplyDamage(Enemy,20.f, GetController(), this, MainDamageType);
			break;
		case 3:
			Cast<UBaseDamageType>(MainDamageType.GetDefaultObject())->HitType = EHitDamageType::EDT_Front;
			UGameplayStatics::ApplyDamage(Enemy,20.f, GetController(), this, MainDamageType);
			break;
		case 0:
			Cast<UBaseDamageType>(MainDamageType.GetDefaultObject())->HitType = EHitDamageType::EDT_Heavy;
			UGameplayStatics::ApplyDamage(Enemy,25.f, GetController(), this, MainDamageType);
			break;
		default: ;
		}
		break;
	case EPlayerAttackMode::EAM_Katana:
		Cast<UBaseDamageType>(MainDamageType.GetDefaultObject())->HitWeapon = EHitWeaponType::EWT_Katana;
		switch (AttackMoveIndex)
		{
		case -1:
			Cast<UBaseDamageType>(MainDamageType.GetDefaultObject())->HitType = EHitDamageType::EDT_ToBack;
			UGameplayStatics::ApplyDamage(Enemy,30.f, GetController(), this, MainDamageType);
			break;
		case 1:
			Cast<UBaseDamageType>(MainDamageType.GetDefaultObject())->HitType = EHitDamageType::EDT_Right;
			UGameplayStatics::ApplyDamage(Enemy,10.f, GetController(), this, MainDamageType);
			break;
		case 2:
			Cast<UBaseDamageType>(MainDamageType.GetDefaultObject())->HitType = EHitDamageType::EDT_Right;
			UGameplayStatics::ApplyDamage(Enemy,15.f, GetController(), this, MainDamageType);
			break;
		case 3:
			Cast<UBaseDamageType>(MainDamageType.GetDefaultObject())->HitType = EHitDamageType::EDT_Heavy;
			UGameplayStatics::ApplyDamage(Enemy,20.f, GetController(), this, MainDamageType);
			break;
		case 0:
			Cast<UBaseDamageType>(MainDamageType.GetDefaultObject())->HitType = EHitDamageType::EDT_ToBack;
			UGameplayStatics::ApplyDamage(Enemy,25.f, GetController(), this, MainDamageType);
			break;
		default: ;
		}
		break;
	default: ;
	}
}

void APlayerCharacter::DeathEnd_Implementation()
{
	UBaseSaveGame* TempSave = Cast<UBaseSaveGame>(UGameplayStatics::CreateSaveGameObject(UBaseSaveGame::StaticClass()));
	if(UGameplayStatics::DoesSaveGameExist(TempSave->SaveName, TempSave->UserIndex))
	{
		TempSave = Cast<UBaseSaveGame>(UGameplayStatics::LoadGameFromSlot(TempSave->SaveName, TempSave->UserIndex));
	}
	if(TempSave->BestScore < Score)
	{
		TempSave->BestScore = Score;
		UGameplayStatics::SaveGameToSlot(TempSave, TempSave->SaveName, TempSave->UserIndex);
	}
	HeroController->PlayerDeath(TempSave->BestScore, Score);
}

void APlayerCharacter::VictoryCheck_Implementation(AActor* Enemy, float ScorePoint, float ExpPoint)
{
	Score += ScorePoint;
	UpdateExp(ExpPoint);
	//Target is dead
	if(TargetEnemy && TargetEnemy == Enemy)
	{
		SetTarget();
	}
	if(DamagedEnemy == Enemy)
	{
		DamagedEnemy = nullptr;
	}
	if(HeroController)
	{
		HeroController->UpdateScore(Score);
	}
}

void APlayerCharacter::SetWeaponTrail_Implementation(bool bEnable)
{
	if(EquippedWeapon)
	{
		IWeaponInterface* WeaponInterface = Cast<IWeaponInterface>(EquippedWeapon);
		if(WeaponInterface)
		{
			WeaponInterface->Execute_SetTrailEnable(EquippedWeapon, bEnable);
		}
	}
}

float APlayerCharacter::GetPlayerScore_Implementation()
{
	return Score;
}

void APlayerCharacter::Hit_Implementation()
{
	if(!IsAlive)
	{
		return ;
	}
	if(HitDamageType->HitType == EHitDamageType::EDT_Still)
	{
		return ;
	}
	
	HitMontage = PlayerHitMontages[AttackMode];
	switch (AttackMode) {
	case EPlayerAttackMode::EAM_NoWeapon:
		if(HitMontage)
		{
			StopAnimMontage();
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
		break;
	case EPlayerAttackMode::EAM_BigSword:
	case EPlayerAttackMode::EAM_Katana:
		if(HitMontage)
		{
			StopAnimMontage();
			switch (HitDamageType->HitType) {
			case EHitDamageType::EDT_Front:
			case EHitDamageType::EDT_Left:
			case EHitDamageType::EDT_Right:
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
				break;
			case EHitDamageType::EDT_Heavy:
				SetActorRotation(FRotator(0,(-HitDirection).Rotation().Yaw,0));
				PlayAnimMontage(HitMontage, 1.f, "KnockDown");
				break;
			case EHitDamageType::EDT_ToBack:
				SetActorRotation(FRotator(0,(-HitDirection).Rotation().Yaw,0));
				PlayAnimMontage(HitMontage, 1.f, "HitToBack");
				break;
			default: ;
			}
		}
		break;
	default: ;
	}
}

void APlayerCharacter::Death_Implementation()
{
	StopAnimMontage();
	IsAlive = false;
	DeathMontage = PlayerDeathMontages[AttackMode];
	if(DeathMontage)
	{
		//Death and facing enemy
		SetActorRotation(FRotator(0,(-HitDirection).Rotation().Yaw,0));
		PlayAnimMontage(DeathMontage);
	}
}

void APlayerCharacter::LevelUp_Implementation()
{
	Super::LevelUp_Implementation();
	Level++;
	MaxMana *= 1.2f;
	Mana = MaxMana;
	MaxStamina *= 1.2f;
	Stamina = MaxStamina;
	MaxExp *= 1.1f;
	if(HeroController)
	{
		HeroController->UpdateHealth(GetHealthPercent());
		HeroController->UpdateMana(GetManaPercent());
		HeroController->UpdateStamina(GetStaminaPercent());
		HeroController->UpdateLevel(Level);
		HeroController->ShowLevelUp(GetMesh()->GetBoneLocation("Pelvis"));
	}
}
