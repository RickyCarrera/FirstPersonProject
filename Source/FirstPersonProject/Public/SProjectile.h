// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SProjectile.generated.h"

class USphereComponent;
class UProjectileMovementComponent;
class UStaticMeshComponent;
class UParticleSystem;
class UDamageType;


UCLASS()
class FIRSTPERSONPROJECT_API ASProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASProjectile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/* Function that will handle the projectile Damaging */
	UFUNCTION()
	void Explode();

protected:

	/* Component that will cause the OnHit function */
	UPROPERTY(VisibleDefaultsOnly, Category = "Projectile")
	USphereComponent* CollisionComponent;

	/* Component that be used for radial damage */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Projectile")
	USphereComponent* DamageRangeCollision;

	/* Updates the position of the Projectile */
	UPROPERTY(VisibleAnywhere, Category = "Movement")
	UProjectileMovementComponent* ProjectileMovement;

	/* Visual representation of the projectile */
	UPROPERTY(EditDefaultsOnly, Category = "Mesh")
	UStaticMeshComponent* ProjectileMesh;

	/* Effect that will be seen upon hit */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Projectile")
	UParticleSystem* ExplosionEffect;

	/* Maximum Speed */
	UPROPERTY(EditDefaultsOnly, Category = "Projectile")
	float ProjectileSpeed;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	TSubclassOf<UDamageType> DamageType;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	/* Executed when our projectile hits something */
	UFUNCTION()
	void OnProjectileHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	USphereComponent* GetCollisionComponent() const { return CollisionComponent; }

	UProjectileMovementComponent* GetProjectileMovement() const { return ProjectileMovement; }
};
