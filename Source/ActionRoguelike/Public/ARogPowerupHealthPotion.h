// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ARogPowerup.h"
#include "ARogPowerupHealthPotion.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API AARogPowerupHealthPotion : public AARogPowerup
{
	GENERATED_BODY()

public:

	AARogPowerupHealthPotion();

	void Interact_Implementation(AActor* InstigatorPawn) override;

protected:

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UStaticMeshComponent* MeshComp;

	// float healt amount?
	
};
