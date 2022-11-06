// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actions/ARogActionEffect.h"
#include "ARogActionEffectThorns.generated.h"

class UARogAttributeComponent;
/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API UARogActionEffectThorns : public UARogActionEffect
{
	GENERATED_BODY()

protected:

	UPROPERTY(EditDefaultsOnly, Category = "Thorns")
	float ReflectFraction;

	UFUNCTION()
	void OnHealthChange(AActor* InstigatorActor, UARogAttributeComponent* OwningComp, float NewHealth, float Delta);

public:

	void StartAction_Implementation(AActor* Instigator) override;

	void StopAction_Implementation(AActor* Instigator) override;

	UARogActionEffectThorns();
	
};
