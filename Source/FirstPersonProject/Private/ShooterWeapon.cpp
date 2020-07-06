// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterWeapon.h"
#include "Components/SkeletalMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "TimerManager.h"


// Sets default values
AShooterWeapon::AShooterWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create the instance of the weapon mesh
	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
	RootComponent = WeaponMesh;


	// defaults
	BulletSpread = 0.1;
	RateOfFire = 600;
	BaseDamage = 45.0f;

	// Sockets
	MuzzleSocketName = "MuzzleSocket";

	// Ammo default
	MaxLoadedAmmo = 30;
}

// Called when the game starts or when spawned
void AShooterWeapon::BeginPlay()
{
	Super::BeginPlay();

	CurrentNumOfBullets = MaxLoadedAmmo;

	TimeBetweenShots = 60.0f / RateOfFire;
	
}


// Called every frame
void AShooterWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void AShooterWeapon::Fire()
{
	if (CurrentNumOfBullets <= 0) { return; }

	// The actor that will have possesion of the fire arm
	AActor* MyOwner = GetOwner();
	if (MyOwner)
	{
		FVector EyeLocation;
		FRotator EyeRotation;
		MyOwner->GetActorEyesViewPoint(EyeLocation, EyeRotation); // returns the point of view of the Actor
		FVector ShotDirection = EyeRotation.Vector();

		// Bullet spread for less accuracy
		float HalfRad = FMath::DegreesToRadians(BulletSpread);
		ShotDirection = FMath::VRandCone(ShotDirection, HalfRad, HalfRad);

		FVector TraceEnd = EyeLocation + (ShotDirection * 10000); // the distance our shot will travel to find a target

		FCollisionQueryParams QueryParams;
		QueryParams.AddIgnoredActor(MyOwner);
		QueryParams.AddIgnoredActor(this);
		QueryParams.bTraceComplex = true; // allows for better accuracy for hitting shots

		FVector TracerEndPoint = TraceEnd;

		FHitResult HitResults; // contains all the infomation about our hit target

		if (GetWorld()->LineTraceSingleByChannel(HitResults, EyeLocation, TraceEnd, ECollisionChannel::ECC_Visibility, QueryParams))
		{
			// Will only enter if we had a blocking hit
			AActor* HitActor = HitResults.GetActor();

			// Applying damage for a hitting shot
			UGameplayStatics::ApplyPointDamage(HitActor, BaseDamage, ShotDirection, HitResults, MyOwner->GetInstigatorController(), MyOwner, DamageType);

			TracerEndPoint = HitResults.ImpactPoint;
		}

		DrawDebugLine(GetWorld(), EyeLocation, TraceEnd, FColor::Green, false, 1.0, 0, 1.0f);
		FireEffects();

		LastTimeFired = GetWorld()->TimeSeconds; // keep track of time since the last shot.

		CurrentNumOfBullets--;
	}
}

void AShooterWeapon::StartFire()
{
	// Will return the higher of the two numbers
	float firstDelay = FMath::Max(LastTimeFired + TimeBetweenShots - GetWorld()->TimeSeconds, 0.0f);
	GetWorldTimerManager().SetTimer(TimerHandle_TimeBetweenShots, this, &AShooterWeapon::Fire, TimeBetweenShots, true, firstDelay);
}

void AShooterWeapon::StopFire()
{
	GetWorldTimerManager().ClearTimer(TimerHandle_TimeBetweenShots);
}

int32 AShooterWeapon::Reload(int32 ReloadAmount)
{
	int32 AmmoToReload = MaxLoadedAmmo - CurrentNumOfBullets;
	if (ReloadAmount < AmmoToReload)
	{
		return ReloadAmount;
	}
	else
	{
		CurrentNumOfBullets += AmmoToReload;
		return AmmoToReload;
	}
}

void AShooterWeapon::FireEffects()
{
	if (MuzzleEffect)
	{
		UGameplayStatics::SpawnEmitterAttached(MuzzleEffect, WeaponMesh, MuzzleSocketName);
	}

	// TODO: add tracer effect

}


