// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SHealthComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FIRSTPERSONPROJECT_API USHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USHealthComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	/* Called when the actor is damaged in any way */
	UFUNCTION()
	void TakeDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);

protected:

	/* Character's default(max) health */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Health")
	float DefaultHealth;

	/* Character's current health */
	UPROPERTY(BlueprintReadOnly)
	float Health;



public:	
	// Called every frame
	//virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
