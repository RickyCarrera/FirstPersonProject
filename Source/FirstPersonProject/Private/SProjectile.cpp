// Fill out your copyright notice in the Description page of Project Settings.


#include "SProjectile.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"

// Sets default values
ASProjectile::ASProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Collision Component that will handle hit events
	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComponent"));
	CollisionComponent->InitSphereRadius(10.0f);
	RootComponent = CollisionComponent;
	CollisionComponent->BodyInstance.SetCollisionProfileName("Projectile");
	CollisionComponent->OnComponentHit.AddDynamic(this, &ASProjectile::OnProjectileHit);

	// Player can't walk on or over the projectile
	CollisionComponent->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.0f));
	CollisionComponent->CanCharacterStepUpOn = ECB_No;

	ProjectileSpeed = 2000.0f;

	// Projectile Movement Component
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
	ProjectileMovement->SetUpdatedComponent(CollisionComponent); // "Update Component (ticking)", Our projectile movement comp will update our collision component.
	ProjectileMovement->InitialSpeed = ProjectileSpeed;
	ProjectileMovement->MaxSpeed = ProjectileSpeed;
	ProjectileMovement->bRotationFollowsVelocity = true; // this projectile will have its rotation updated each frame to match the direction of its velocity.
	

	// Projectile Mesh
	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileMesh"));
	ProjectileMesh->SetupAttachment(RootComponent);

}

// Called when the game starts or when spawned
void ASProjectile::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASProjectile::Explode()
{
	if (ensure(ExplosionEffect))
	{
		FVector BlastLocation = this->GetActorLocation();
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplosionEffect, GetActorLocation(), GetActorRotation());
		TArray<AActor*> IgnoreActors;
		UGameplayStatics::ApplyRadialDamageWithFalloff(GetWorld(), 50.0f, 10.0f, GetActorLocation(), 50.0f, 500.0f, 1.0, DamageType, IgnoreActors, this, GetInstigatorController());
		DrawDebugSphere(GetWorld(), BlastLocation, 150.0f, 12, FColor::Red, false, 1.0f);
		Destroy();
	}
}

void ASProjectile::OnProjectileHit(UPrimitiveComponent * HitComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, FVector NormalImpulse, const FHitResult & Hit)
{
	if ((OtherActor != NULL) && (OtherActor != this))
	{
		Explode();
	}
}

