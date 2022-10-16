// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ARogProjectileBase.h"
#include "ARogTeleportProjectile.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API AARogTeleportProjectile : public AARogProjectileBase
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

	virtual void PostInitializeComponents() override;

	// Handle the Teleport Time
	FTimerHandle TeleportTimerHandle;

	// Teleport Instigator
	void Teleport();

	void PlayEffects();

public:
	void OnComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
};
