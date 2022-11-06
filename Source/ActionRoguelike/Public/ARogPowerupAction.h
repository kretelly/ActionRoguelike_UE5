// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ARogPowerup.h"
#include "ARogPowerupAction.generated.h"

class UARogActionObject;

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API AARogPowerupAction : public AARogPowerup
{
	GENERATED_BODY()

protected:

	UPROPERTY(EditAnywhere, Category = "Powerup")
	TSubclassOf<UARogActionObject> ActionToGrant;

public:

	void Interact_Implementation(APawn* InstigatorPawn) override;

};
