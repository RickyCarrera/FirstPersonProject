// Fill out your copyright notice in the Description page of Project Settings.


#include "SProjectileWeapon.h"
#include "Components/SkeletalMeshComponent.h"
#include "SProjectile.h"
#include "TimerManager.h"

void ASProjectileWeapon::Fire()
{
	AActor* MyOwner = GetOwner();

	if (ensure(ProjectileClass) && ensure(MyOwner))
	{
		//We want to fire from the muzzle location, but at an angle based on the camera, not the muzzle
		FVector EyeLocation;
		FRotator EyeRotation;
		MyOwner->GetActorEyesViewPoint(EyeLocation, EyeRotation);

		FVector MuzzleLocation = WeaponMesh->GetSocketLocation(MuzzleSocketName);

		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		GetWorld()->SpawnActor<AActor>(ProjectileClass, MuzzleLocation, EyeRotation, SpawnParams);

		LastTimeFired = GetWorld()->TimeSeconds;
	}
}

void ASProjectileWeapon::StartFire()
{
	float FirstDelay = FMath::Max((LastTimeFired + TimeBetweenShots - GetWorld()->TimeSeconds), 0.0f);
	GetWorldTimerManager().SetTimer(TimerHandle_TimeBetweenShots, this, &ASProjectileWeapon::Fire, TimeBetweenShots, true, FirstDelay);

}

void ASProjectileWeapon::StopFire()
{
	GetWorldTimerManager().ClearTimer(TimerHandle_TimeBetweenShots);
}
