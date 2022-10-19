// Fill out your copyright notice in the Description page of Project Settings.


#include "ARogTeleportProjectile.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Particles/ParticleSystemComponent.h"

AARogTeleportProjectile::AARogTeleportProjectile()
{
	TeleportDelay = 0.2f;
	DetonateDelay = 0.2f;
	Projectile->InitialSpeed = 6000.f;
}

void AARogTeleportProjectile::BeginPlay()
{
	Super::BeginPlay();
	GetWorldTimerManager().SetTimer(DetonationDelayTimerHandle, this, &AARogTeleportProjectile::Explode, DetonateDelay);
}

void AARogTeleportProjectile::Explode_Implementation()
{
	// Clear timer if the Explode was already called through another source like OnActorHit
	GetWorldTimerManager().ClearTimer(DetonationDelayTimerHandle);

	UGameplayStatics::SpawnEmitterAtLocation(this, HitParticle, GetActorLocation(), GetActorRotation());

	ParticleSysComp->DeactivateSystem();

	Projectile->StopMovementImmediately();
	SetActorEnableCollision(false);

	FTimerHandle TeleportDelayTimerHandle;
	GetWorldTimerManager().SetTimer(TeleportDelayTimerHandle, this, &AARogTeleportProjectile::TeleportInstigator, TeleportDelay);

	// Skip base implementation as it will destroy actor
	//Super::Explode_Implementation();
}

void AARogTeleportProjectile::TeleportInstigator()
{
	AActor* ActorToTeleport = GetInstigator();
	if (ensure(ActorToTeleport))
	{
		// Keep instigator rotation or it may end up jarring
		ActorToTeleport->TeleportTo(GetActorLocation(), ActorToTeleport->GetActorRotation(), false, false);
	}
}
