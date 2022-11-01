// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ARogPowerup.h"
#include "ARogPowerupCredits.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API AARogPowerupCredits : public AARogPowerup
{
	GENERATED_BODY()

protected:

	UPROPERTY(EditAnywhere, Category = "Credits")
	int32 CreditsAmount;

public:

	void Interact_Implementation(APawn* InstigatorPawn) override;

	AARogPowerupCredits();

};
