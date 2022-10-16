// Fill out your copyright notice in the Description page of Project Settings.


#include "ARogTeleportProjectile.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/ProjectileMovementComponent.h"

void AARogTeleportProjectile::BeginPlay()
{
	Super::BeginPlay();
	GetWorld()->GetTimerManager().SetTimer(TeleportTimerHandle, this, &AARogTeleportProjectile::Teleport, 0.3f, false);
	SetLifeSpan(1.0f);
}

void AARogTeleportProjectile::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	Projectile->InitialSpeed = 7000000.0f;
}

void AARogTeleportProjectile::OnComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	GetWorld()->GetTimerManager().ClearTimer(TeleportTimerHandle);
	
	Projectile->StopMovementImmediately();
	
	PlayEffects();
}

void AARogTeleportProjectile::Teleport()
{
	Projectile->StopMovementImmediately();

	PlayEffects();

	GetInstigator()->TeleportTo(GetActorLocation(), GetInstigator()->GetActorRotation());
}

void AARogTeleportProjectile::PlayEffects()
{
	UGameplayStatics::SpawnEmitterAtLocation(this, HitParticle, GetActorLocation(), GetActorRotation());
	// Play sound
	GEngine->AddOnScreenDebugMessage(-1, 2.5f, FColor::Blue, "Fire Sound");
	// Play camera shake
	GEngine->AddOnScreenDebugMessage(-1, 2.5f, FColor::Blue, "Fire CameraShake");
	// Destroy itself
	Destroy();
}