// Fill out your copyright notice in the Description page of Project Settings.


#include "MainAnimInstance.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Character.h"

void UMainAnimInstance::NativeInitializeAnimation()
{
	if (Pawn == nullptr)
	{
		Pawn = TryGetPawnOwner(); // will retrieve the owner of this animation instance

	}

}

// Function that can get called everyframe
void UMainAnimInstance::UpdateAnimationProperties()
{
	if (Pawn == nullptr)
	{
		Pawn = TryGetPawnOwner();
	}

	if (Pawn)
	{
		FVector Speed = Pawn->GetVelocity(); // will return our speed
		FVector LateralSpeed = FVector(Speed.X, Speed.Y, 0.f);
		MovementSpeed = LateralSpeed.Size(); // sets MovementSpeed to the magnitude of LateralSpeed 

		bIsInAir = Pawn->GetMovementComponent()->IsFalling();

		ACharacter* Character = Cast<ACharacter>(Pawn);
		if (Character)
		{
			bCrouching = Character->bIsCrouched;
			bRunJump = Character->bWasJumping;
		}
	}
}
