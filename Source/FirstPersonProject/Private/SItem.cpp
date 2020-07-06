// Fill out your copyright notice in the Description page of Project Settings.


#include "SItem.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"

// Sets default values
ASItem::ASItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Collision Volume
	CollisionVolume = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionVolume"));
	RootComponent = CollisionVolume;

	// Item Mesh
	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BaseMesh"));
	BaseMesh->SetupAttachment(RootComponent);

	// Rotation defaults
	bRotate = false;
	RotationRate = 45.0f;

}

// Called when the game starts or when spawned
void ASItem::BeginPlay()
{
	Super::BeginPlay();

	CollisionVolume->OnComponentBeginOverlap.AddDynamic(this, &ASItem::OnOverlapBegin);
	CollisionVolume->OnComponentEndOverlap.AddDynamic(this, &ASItem::OnOverlapEnd);
	
}

// Called every frame
void ASItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bRotate)
	{
		FRotator Rotation = GetActorRotation();
		Rotation.Yaw += DeltaTime * RotationRate;
		SetActorRotation(Rotation);
	}

}


void ASItem::OnOverlapBegin(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{

}

void ASItem::OnOverlapEnd(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex)
{

}

