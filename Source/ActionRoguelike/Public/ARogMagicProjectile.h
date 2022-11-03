// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ARogProjectileBase.h"
#include "GameplayTagContainer.h"
#include "ARogMagicProjectile.generated.h"

class USphereComponent;
class UParticleSystemComponent;
class UProjectileMovementComponent;
class UARogActionEffect;

UCLASS()
class ACTIONROGUELIKE_API AARogMagicProjectile : public AARogProjectileBase // Re-parented from AActor
{
	GENERATED_BODY()
	
public:	
	AARogMagicProjectile();

protected:

	UPROPERTY(EditDefaultsOnly, Category="Damage")
	float DamageAmount;
	
	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	FGameplayTag ParryTag;

	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	TSubclassOf<UARogActionEffect> BurningActionClass;

	virtual void PostInitializeComponents() override;

	UFUNCTION()
	void OnActorBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
