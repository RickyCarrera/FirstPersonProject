// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/UserDefinedEnum.h"
#include "AmmoTypeEnum.generated.h"

/**
 * 
 */
UENUM(BlueprintType)
enum class EAmmoType : uint8
{
	AMMO_Rifle,
	AMMO_Projectile
};

UCLASS()
class FIRSTPERSONPROJECT_API UAmmoTypeEnum : public UUserDefinedEnum
{
	GENERATED_BODY()

	
};
