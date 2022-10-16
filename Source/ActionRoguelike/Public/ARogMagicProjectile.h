// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ARogProjectileBase.h"
#include "ARogMagicProjectile.generated.h"

class USphereComponent;
class UParticleSystemComponent;
class UProjectileMovementComponent;

UCLASS()
class ACTIONROGUELIKE_API AARogMagicProjectile : public AARogProjectileBase // Re-parented from AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AARogMagicProjectile();

protected:
	
	// Called when the game starts or when spawned
	//virtual void BeginPlay() override;

	virtual void PostInitializeComponents() override;

public:	
	// Called every frame
	//virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void OnActorBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
