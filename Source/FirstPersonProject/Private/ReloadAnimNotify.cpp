// Fill out your copyright notice in the Description page of Project Settings.


#include "ReloadAnimNotify.h"

#include "MainCharacter.h"

#include "Components/SkeletalMeshComponent.h"

void UReloadAnimNotify::Notify(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation)
{
	AActor* AnimationOwner = MeshComp->GetOwner();
	AMainCharacter* Character = Cast<AMainCharacter>(AnimationOwner);
	if(Character)
	{
		Character->bIsReloading = false;
	}
}
