// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ShooterWeapon.generated.h"

class USkeletalMeshComponent;
class UDamageType;
class UParticleSystem;

UCLASS()
class FIRSTPERSONPROJECT_API AShooterWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AShooterWeapon();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/* Function that handles firing the weapon */
	virtual void Fire();

	/* Handles the effects that spawn for our weapon being fired*/
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void FireEffects();

protected:

	/* Visual representation of the weapon class */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh")
	USkeletalMeshComponent* WeaponMesh;

	/* DamageType used for line tracing */
	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	TSubclassOf<UDamageType> DamageType;

	/* Muzzle flash particle */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "FX")
	UParticleSystem* MuzzleEffect;

	/* Rounds per minute */
	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	float RateOfFire;

	/* Records the time between shots */
	FTimerHandle TimerHandle_TimeBetweenShots;

	/* the time of previous fire */
	float LastTimeFired;

	/* derived from RateOfFire */
	float TimeBetweenShots;

	/* Bullet spread in degrees */
	UPROPERTY(EditDefaultsOnly, Category = "Weapon", meta = (ClampMin = 0.0f))
	float BulletSpread;

	/* Base damage of a hitting shot */
	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	float BaseDamage;

	/* Muzzle Socket */
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	FName MuzzleSocketName;

	/* Clip size of the weapon, amount declare in BP */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ammo")
	int32 MaxLoadedAmmo;

	/* Current number of bullets to fire */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ammo")
	int32 CurrentNumOfBullets;

	/* Ref to our Reload Anim Notify */
	UPROPERTY(EditDefaultsOnly, Category = "FX")
	UAnimMontage* ReloadMontage;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void StartFire();
	virtual void StopFire();
	virtual int32 Reload(int32 ReloadAmount);

	int32 GetMaxLoadedAmmo() { return MaxLoadedAmmo; }
	UAnimMontage* GetReloadMontage() { return ReloadMontage; }

};
