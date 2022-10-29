// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actions/ARogActionObject.h"
#include "ARogActionProjectileAttack.generated.h"

class UAnimMontage;
class UParticleSystem;

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API UARogActionProjectileAttack : public UARogActionObject
{
	GENERATED_BODY()

protected:

	UPROPERTY(EditDefaultsOnly, Category = Attack)
	TSubclassOf<AActor> ProjectileClass;

	UPROPERTY(EditDefaultsOnly, Category = Attack)
	UAnimMontage* AttackAnim;

	UPROPERTY(EditAnywhere, Category = Attack)
	UParticleSystem* CastingEffect; // Played attached during the attack animation

	/* Instead of use timer we should use Animation Notify at this case (Change it)*/
	UPROPERTY(EditDefaultsOnly, Category = Attack)
	float AttackAnimDelay;

	UPROPERTY(VisibleAnywhere, Category = Effects)
	FName HandSocketName;

	UFUNCTION()
	void AttackTimerElapsed(ACharacter* IntigatorCharacter);

public:
	
	UARogActionProjectileAttack();

	virtual void StartAction_Implementation(AActor* Instigator) override;

};
