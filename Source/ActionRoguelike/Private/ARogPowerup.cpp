// Fill out your copyright notice in the Description page of Project Settings.

#include "ARogPowerup.h"
#include "Components/SphereComponent.h"

// Sets default values
AARogPowerup::AARogPowerup()
{
	SphereComp = CreateDefaultSubobject<USphereComponent>("SphereComp");
	SphereComp->SetCollisionProfileName("Powerup");
	RootComponent = SphereComp;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>("MeshComp");
	// Disable collision, instead we use SphereComp to handle interaction queries
	MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	MeshComp->SetupAttachment(RootComponent);

	RespawnTime = 10.0f;

	// Allow replicate when spawned during gameplay
	SetReplicates(true); 
}

void AARogPowerup::Interact_Implementation(APawn* InstigatorPawn)
{
	// logic in derived classes...
}

void AARogPowerup::ShowPowerup()
{
	SetPowerupState(true);
}

void AARogPowerup::SetPowerupState(bool bNewIsActive)
{
	SetActorEnableCollision(bNewIsActive);
	// Set visibility on root and all children
	RootComponent->SetVisibility(bNewIsActive, true);
}

void AARogPowerup::HideAndCooldownPowerup()
{
	SetPowerupState(false);

	FTimerHandle TimerHandle_RespawnTimer;
	GetWorldTimerManager().SetTimer(TimerHandle_RespawnTimer, this, &AARogPowerup::ShowPowerup, RespawnTime);
}
