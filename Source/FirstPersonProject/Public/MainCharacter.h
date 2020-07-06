// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MainCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;
class AShooterWeapon;

UCLASS()
class FIRSTPERSONPROJECT_API AMainCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMainCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/* Handles switching to the Rifle */
	UFUNCTION()
	void SwitchToRifle();

	/* Handles switching to the Launcher */
	UFUNCTION()
	void SwitchToLauncher();

	/* Handles Forward and Backward movement */
	UFUNCTION()
	void MoveForward(float value);

	/* Handles Right and Left movement */
	UFUNCTION()
	void MoveRight(float value);
	
	/* Sets the jump flag when the key is presssed */
	//UFUNCTION()
	//void StartJump();

	/* Clears the jump flag when the key is released */
	//UFUNCTION()
	//void StopJump();

	/* Sets the crouch flag when the key is pressed */
	UFUNCTION()
	void StartCrouch();

	/* Clears the crouch flag when the key is released */
	UFUNCTION()
	void StopCrouch();

	/* Will start the aim down sights */
	void BeginZoom();

	/* Will stop the aim down sights */
	void EndZoom();

	/* Double jump function */
	UFUNCTION()
	void DoubleJump();

	/* Gets Called when our player lands on the ground and will be used to reset our DoubleJumpCounter */
	virtual void Landed(const FHitResult& Hit) override;

	/* Handles sprint mechanics */
	UFUNCTION()
	void Sprint();

	/* Handles walk mechanics */
	UFUNCTION()
	void Walk();

	/* Handles Dash mechanics */
	UFUNCTION()
	void Dash();

	/* Function handles timer for Dashing */
	UFUNCTION()
	void StopDashing();

	/* Function handles reseting timer for dashing */
	UFUNCTION()
	void ResetDashCooldown();

	/* Handles the call to reload current weapon */
	UFUNCTION()
	void Reload();
protected:

	/* Third person camera that will follow character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UCameraComponent* CameraComponent;

	/* Spring Arm Component that will prevent clipping */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USpringArmComponent* SpringArmComponent;


	/* Container for our weapon to spawn */
	UPROPERTY(EditDefaultsOnly, Category = "Player")
	TArray<TSubclassOf<AShooterWeapon>> WeaponList;

	/* Weapon that our character will hold */
	UPROPERTY(BlueprintReadOnly)
	AShooterWeapon* CurrentWeapon;

	/* Container to add weapons that we have equipped */
	TArray<AShooterWeapon*> EquippedWeapons;

	/* The weapon class that we will have to choose from and spawn with */
	// UPROPERTY(EditDefaultsOnly, Category = "Player")
	// TSubclassOf<AShooterWeapon> StarterWeaponClass;

	/* Socket that Weapon will be attached to */
	UPROPERTY(VisibleDefaultsOnly, Category = "Player")
	FName WeaponAttachedSocket;

	/* Bool for Aim down sights */
	UPROPERTY(BlueprintReadOnly)
	bool bWantsToZoom;

	/* The amount that our view will zoom when triggered */
	float ZoomedFov;

	/* Default view (FOV), set during BeginPlay */
	float DefaultFov;

	/* Smoothness for the transition between default zoom and zoomed in */
	UPROPERTY(EditDefaultsOnly, Category = "Player", meta = (ClampMin = 0.1, Clamp = 100))
	float ZoomInterpSpeed;

	/* Will store the amount of jumps we are allowed to have */
	UPROPERTY()
	int DoubleJumpCounter;

	/* adjustable jump height */
	UPROPERTY(EditDefaultsOnly, Category = "Movement")
	float JumpHeight;

	/* adjustable sprinting speed */
	UPROPERTY(EditDefaultsOnly, Category = "Movement")
	float SprintSpeed;

	/* adjustable walking speed */
	UPROPERTY(EditDefaultsOnly, Category = "Movement")
	float WalkSpeed;

	/* adjustable dashing distance */
	UPROPERTY(EditDefaultsOnly, Category = "Movement")
	float DashDistance;

	/* adjustable dashing cooldown */
	UPROPERTY(EditDefaultsOnly, Category = "Movement")
	float DashCooldown;

	/* Will set the flag to see if character can dash */
	UPROPERTY()
	bool bCanDash;

	/* Adjustable waiting period between dashes */
	UPROPERTY(EditAnywhere)
	float DashStop;

	UPROPERTY()
	FTimerHandle UnUsedTimerHandle;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ammo")
	int32 MaxRifleAmmo;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ammo")
	int32 MaxProjectileAmmo;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/* Returns our Pawn's eye location orginally but will return our camera component location instead */
	virtual FVector GetPawnViewLocation() const override;
	
	/* Function that will handle the firing of our weapon */
	UFUNCTION(BlueprintCallable, Category = "Player")
	void StartFire();
	/* Function that will clear the firing flag to stop firing weapon */
	UFUNCTION(BlueprintCallable, Category = "Player")
	void StopFire();

	/* bool used to set or clear reload flag */
	UPROPERTY(BlueprintReadOnly)
	bool bIsReloading;

};
