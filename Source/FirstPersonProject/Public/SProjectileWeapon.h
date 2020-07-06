// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ShooterWeapon.h"
#include "SProjectileWeapon.generated.h"

/**
 * 
 */
UCLASS()
class FIRSTPERSONPROJECT_API ASProjectileWeapon : public AShooterWeapon
{
	GENERATED_BODY()
protected:
	virtual void Fire() override;

	UPROPERTY(EditDefaultsOnly, Category = "Projectiles")
	TSubclassOf<AActor> ProjectileClass;

public:

	/* Function that will handle the firing of our weapon */
	virtual void StartFire() override;
	/* Function that will clear the firing flag to stop firing weapon */
	virtual void StopFire() override;
	
};
