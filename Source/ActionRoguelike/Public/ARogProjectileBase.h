// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ARogProjectileBase.generated.h"

class USphereComponent;
class UParticleSystemComponent;
class UProjectileMovementComponent;
class UAudioComponent;
class USoundCue;
class UCameraShakeBase;

UCLASS()
class ACTIONROGUELIKE_API AARogProjectileBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AARogProjectileBase();

protected:

	// Base collision shape
	UPROPERTY(VisibleAnywhere)
	USphereComponent* SphereComp;

	// Effects
	UPROPERTY(VisibleAnywhere)
	UParticleSystemComponent* ParticleSysComp;

	// Projectile Movement
	UPROPERTY(VisibleAnywhere)
	UProjectileMovementComponent* Projectile;

	// Audio Component
	UPROPERTY(VisibleAnywhere)
	UAudioComponent* AudioComp;

	UPROPERTY(EditDefaultsOnly)
	USoundCue* ImpactSound;

	// Spawned Particle on Hit Event
	UPROPERTY(EditDefaultsOnly, Category = Effects)
	UParticleSystem* HitParticle;

	virtual void PostInitializeComponents() override;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void Explode();

	UPROPERTY(EditDefaultsOnly, Category = CameraShake)
	TSubclassOf<UCameraShakeBase> SimpleCameraShake;

	UPROPERTY(EditDefaultsOnly, Category = CameraShake)
	float ImpactShakeInnerRadius;

	UPROPERTY(EditDefaultsOnly, Category = CameraShake)
	float ImpactShakeOuterRadius;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void OnComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
};
