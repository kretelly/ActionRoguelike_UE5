// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ARogGameplayInterface.h"
#include "ARogPowerup.generated.h"

class USphereComponent;

UCLASS()
class ACTIONROGUELIKE_API AARogPowerup : public AActor, public IARogGameplayInterface
{
	GENERATED_BODY()
	
public:	
	
	AARogPowerup();

	void Interact_Implementation(AActor* InstigatorPawn) override;

protected:

	UPROPERTY(VisibleAnywhere, Category = "Components")
	USphereComponent* SphereComp;

	UPROPERTY(EditAnywhere, Category = "Powerup")
	float RespawnTime;

	UFUNCTION()
	void ShowPowerup();

	void HideAndCooldownPowerup();

	void SetPowerupState(bool bNewIsActive);

};
