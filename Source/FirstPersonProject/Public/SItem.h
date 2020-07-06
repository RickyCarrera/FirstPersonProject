// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SItem.generated.h"

class USphereComponent;
class UStaticMeshComponent;
UCLASS()
class FIRSTPERSONPROJECT_API ASItem : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASItem();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

protected:

	/* Base Shape for the Collision Component */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Item | Collision")
	USphereComponent* CollisionVolume;

	/* Visual representation of the class */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Item | Mesh")
	UStaticMeshComponent* BaseMesh;

	/* Possible TODO:
	   add idleparticleComponent,
	   add OverlapParticles,
	   add OverlapSounds
	*/

	/* bool that sets the flag to have our item rotate */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Item | ItemProperties")
	bool bRotate;

	/* Adjustable rotation rate */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item | ItemProperties")
	float RotationRate;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	/* Handles the CollisionVolume overlapping events */
	UFUNCTION()
	virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	/* Handles the CollisionVolume non-overlapping events */
	UFUNCTION()
	virtual void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

};
