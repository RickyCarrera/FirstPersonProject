// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "MainAnimInstance.generated.h"

class APawn;
/**
 * 
 */
UCLASS()
class FIRSTPERSONPROJECT_API UMainAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:

	/* Keeps track of how fast our character is moving */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
	float MovementSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
	/* will set or clear the jump flag and respond to whether our character is jumping or now */
	bool bIsInAir;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
	/* will set or clear the crouching flag and respond to whether our character is crouching or not */
	bool bCrouching;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
	/* hacked together */
	bool bRunJump;

	/* reference to the pawn */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
	APawn* Pawn;

	UPROPERTY(BlueprintReadOnly)
	bool bIsReloading;

public:

	/* AnimInstance verison of a 'BeginPlay' */
	UFUNCTION()
	virtual void NativeInitializeAnimation() override;

	/* Will keep track and update MovementSpeed and bIsInAir frame by frame */
	UFUNCTION(BlueprintCallable, Category = "AnimationProperties")
	void UpdateAnimationProperties();
};
