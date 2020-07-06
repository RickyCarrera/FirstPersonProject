// Fill out your copyright notice in the Description page of Project Settings.


#include "MainCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "..\Public\ShooterWeapon.h"
#include "Engine/World.h"
#include "TimerManager.h"

// Sets default values
AMainCharacter::AMainCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	// Instantiate the spring arm component
	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	SpringArmComponent->SetupAttachment(RootComponent);
	SpringArmComponent->bUsePawnControlRotation = true;

	// Instantiate the camera component
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	CameraComponent->SetupAttachment(SpringArmComponent);

	// Enable support for crouch for our player
	GetMovementComponent()->GetNavAgentPropertiesRef().bCanCrouch = true;

	// Socket
	WeaponAttachedSocket = "WeaponSocket";

	// defaults for zoom fov
	ZoomedFov = 65.0f;
	ZoomInterpSpeed = 20.0f;

	// Jump height default
	JumpHeight = 600.0f;

	// Defaults for Walking and Sprinting Speed
	WalkSpeed = 600.0f;
	SprintSpeed = 1000.f;

	// Defaults for Dashing
	bCanDash = true;
	DashDistance = 6000.0f;
	DashCooldown = 1.0f;
	DashStop = 0.1;

	// Ammo defaults
	MaxRifleAmmo = 300;
	MaxProjectileAmmo = 10;

}

// Called when the game starts or when spawned
void AMainCharacter::BeginPlay()
{
	Super::BeginPlay();

	DefaultFov = CameraComponent->FieldOfView;
	
	//Spawn a Default Weapon
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	if (WeaponList.Num() > 0)
	{
		for (int i = 0; i < WeaponList.Num(); i++)
		{
			AShooterWeapon* Weapon = GetWorld()->SpawnActor<AShooterWeapon>(WeaponList[i].Get(), FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);
			Weapon->SetOwner(this);
			Weapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, WeaponAttachedSocket);
			Weapon->SetActorHiddenInGame(true);
			EquippedWeapons.Add(Weapon);
		}
		CurrentWeapon = EquippedWeapons[0];
		CurrentWeapon->SetActorHiddenInGame(false);
	}
}

// Called every frame
void AMainCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	float TargetFov = bWantsToZoom ? ZoomedFov : DefaultFov;

	float NewFov = FMath::FInterpTo(CameraComponent->FieldOfView, TargetFov, DeltaTime, ZoomInterpSpeed);
	CameraComponent->SetFieldOfView(NewFov); // default is 90

}

void AMainCharacter::MoveForward(float value)
{
	// Find out which way is "Forward" and record that the player wants to move that way
	FVector Direction = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::X);
	AddMovementInput(Direction,value);
}

void AMainCharacter::MoveRight(float value)
{
	// Find out which way is "Right" and record that the player wants to move that way
	FVector Direction = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::Y);
	AddMovementInput(Direction, value);
}

/*
void AMainCharacter::StartJump()
{
	bPressedJump = true;
}

void AMainCharacter::StopJump()
{

	bPressedJump = false;
}
*/

void AMainCharacter::DoubleJump()
{
	if (DoubleJumpCounter <= 1)
	{
		FVector LaunchVelocity(0, 0, JumpHeight);
		ACharacter::LaunchCharacter(LaunchVelocity, false, true);
		DoubleJumpCounter++;
	}
}

void AMainCharacter::Landed(const FHitResult & Hit)
{
	DoubleJumpCounter = 0; // will be reset everytime our character lands
}

void AMainCharacter::Sprint()
{
	GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
}

void AMainCharacter::Walk()
{
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
}

void AMainCharacter::Dash()
{
	if (bCanDash)
	{
		GetCharacterMovement()->BrakingFrictionFactor = 0.f; // will enable character not to slow down during dash in it touches anything
		FVector LaunchVelocity = FVector(CameraComponent->GetForwardVector().X, CameraComponent->GetForwardVector().Y, 0).GetSafeNormal();
		LaunchCharacter(LaunchVelocity * DashDistance, true, true);
		bCanDash = false;
		GetWorldTimerManager().SetTimer(UnUsedTimerHandle, this, &AMainCharacter::StopDashing, DashStop, false);
	}
}

void AMainCharacter::StopDashing()
{
	GetCharacterMovement()->StopMovementImmediately();
	GetCharacterMovement()->BrakingFrictionFactor = 2.f;
	GetWorldTimerManager().SetTimer(UnUsedTimerHandle, this, &AMainCharacter::ResetDashCooldown, DashCooldown, false);
}

void AMainCharacter::ResetDashCooldown()
{
	bCanDash = true;

}


void AMainCharacter::StartCrouch()
{
	Crouch();
}

void AMainCharacter::StopCrouch()
{
	UnCrouch();
}

// Called to bind functionality to input
void AMainCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Movement Axis Mappings
	PlayerInputComponent->BindAxis("MoveForward", this, &AMainCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AMainCharacter::MoveRight);
	// Looking Axis Mappings
	PlayerInputComponent->BindAxis("LookUp", this, &AMainCharacter::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("Turn", this, &AMainCharacter::AddControllerYawInput);
	// Jumping Action Mappings
	//PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AMainCharacter::StartJump);
	//PlayerInputComponent->BindAction("Jump", IE_Released, this, &AMainCharacter::StopJump);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AMainCharacter::DoubleJump);
	// Sprint & Walk Action Mapping
	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &AMainCharacter::Sprint);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &AMainCharacter::Walk);
	// Dash Action Mapping
	PlayerInputComponent->BindAction("Dash", IE_Pressed, this, &AMainCharacter::Dash);
	// Crouch Action Mappings
	PlayerInputComponent->BindAction("Crouch", IE_Pressed, this, &AMainCharacter::StartCrouch);
	PlayerInputComponent->BindAction("Crouch", IE_Released, this, &AMainCharacter::StopCrouch);
	// Fire Action Mappings
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &AMainCharacter::StartFire);
	PlayerInputComponent->BindAction("Fire", IE_Released, this, &AMainCharacter::StopFire);
	// Reload Action Mapping
	PlayerInputComponent->BindAction("Reload", IE_Pressed, this, &AMainCharacter::Reload);
	// Zoom Action Mappings
	PlayerInputComponent->BindAction("Zoom", IE_Pressed, this, &AMainCharacter::BeginZoom);
	PlayerInputComponent->BindAction("Zoom", IE_Released, this, &AMainCharacter::EndZoom);
	// Switch weapon Action Mappings
	PlayerInputComponent->BindAction("SwitchToRifle", IE_Pressed, this, &AMainCharacter::SwitchToRifle);
	PlayerInputComponent->BindAction("SwitchToLauncher", IE_Pressed, this, &AMainCharacter::SwitchToLauncher);

}

FVector AMainCharacter::GetPawnViewLocation() const
{
	if (CameraComponent)
	{
		return CameraComponent->GetComponentLocation();
	}

	return Super::GetPawnViewLocation();
}

void AMainCharacter::StartFire()
{
	if (CurrentWeapon && !bIsReloading)
	{
		CurrentWeapon->StartFire();
	}
	else if (CurrentWeapon && bIsReloading)
	{
		StopFire();
	}
}

void AMainCharacter::StopFire()
{
	if (CurrentWeapon)
	{
		CurrentWeapon->StopFire();
	}
}

void AMainCharacter::Reload()
{
	if (!bIsReloading)
	{
		bIsReloading = true;
		StopFire();
		
		if (CurrentWeapon)
		{
			UAnimMontage* ReloadMontage = CurrentWeapon->GetReloadMontage();
			if (ReloadMontage)
			{
				PlayAnimMontage(ReloadMontage);
			}
		}
		bIsReloading = false;
	}
}

void AMainCharacter::BeginZoom()
{
	bWantsToZoom = true;
}

void AMainCharacter::EndZoom()
{
	bWantsToZoom = false;
}

void AMainCharacter::SwitchToRifle()
{
	/*
	CurrentWeapon->Destroy();
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	CurrentWeapon = GetWorld()->SpawnActor<AShooterWeapon>(WeaponList[0].Get(), FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);
	if (CurrentWeapon)
	{
		CurrentWeapon->SetOwner(this);
		CurrentWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, WeaponAttachedSocket);
	}
	*/

	if (!bIsReloading)
	{
		StopFire();
		CurrentWeapon->SetActorHiddenInGame(true);
		CurrentWeapon = EquippedWeapons[0];
		CurrentWeapon->SetActorHiddenInGame(false);
	}
}

void AMainCharacter::SwitchToLauncher()
{
	/*
	CurrentWeapon->Destroy();
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	CurrentWeapon = GetWorld()->SpawnActor<AShooterWeapon>(WeaponList[1].Get(), FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);
	if (CurrentWeapon)
	{
		CurrentWeapon->SetOwner(this);
		CurrentWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, WeaponAttachedSocket);
	}
	*/
	if (!bIsReloading)
	{
		StopFire();
		CurrentWeapon->SetActorHiddenInGame(true);
		CurrentWeapon = EquippedWeapons[1];
		CurrentWeapon->SetActorHiddenInGame(false);
	}

}

