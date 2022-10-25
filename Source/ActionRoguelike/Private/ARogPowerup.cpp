// Fill out your copyright notice in the Description page of Project Settings.

#include "ARogPowerup.h"
#include "Components/SphereComponent.h"

// Sets default values
AARogPowerup::AARogPowerup()
{
	SphereComp = CreateDefaultSubobject<USphereComponent>("SphereComp");
	SphereComp->SetCollisionProfileName("Powerup");
	RootComponent = SphereComp;

	RespawnTime = 10.0f;
}

void AARogPowerup::Interact_Implementation(AActor* InstigatorPawn)
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
