// Fill out your copyright notice in the Description page of Project Settings.


#include "ARogItemChest.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
AARogItemChest::AARogItemChest()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>("BaseMesh");
	SetRootComponent(BaseMesh);

	LidMesh = CreateDefaultSubobject<UStaticMeshComponent>("LidMesh");
	LidMesh->SetupAttachment(BaseMesh);

	TargetPitch = 110.0f;
}

void AARogItemChest::Interact_Implementation(APawn* InstigatorPawn)
{
	LidMesh->SetRelativeRotation(FRotator(TargetPitch, 0.0f, 0.0f));
}

// Called when the game starts or when spawned
void AARogItemChest::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AARogItemChest::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

