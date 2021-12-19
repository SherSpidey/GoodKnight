// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseAnimInstance.h"

#include "GameFramework/PawnMovementComponent.h"

void UBaseAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	if(!Character)
	{
		Character = TryGetPawnOwner();
	}
}

void UBaseAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	UpdateMovementState();
}

void UBaseAnimInstance::UpdateMovementState()
{
	//Maybe not set bind, do this to avoid crash
	if(!Character)
	{
		Character = TryGetPawnOwner();
	}
	if(Character)
	{
		Speed = FVector(Character->GetVelocity().X, Character->GetVelocity().Y, 0.f).Size();
		IsInAir = Character->GetMovementComponent()->IsFalling();
		//Maybe increase the value to avoid terrible animation
		IsMoving = Speed > 0.f;
		Direction = CalculateDirection(Character->GetVelocity(), Character->GetActorRotation());
	}
}
