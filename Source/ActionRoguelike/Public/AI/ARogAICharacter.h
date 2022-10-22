// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ARogAICharacter.generated.h"

class UPawnSensingComponent;

UCLASS()
class ACTIONROGUELIKE_API AARogAICharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AARogAICharacter();

protected:

	virtual void PostInitializeComponents() override;

	UPROPERTY(EditDefaultsOnly, Category = Components)
	UPawnSensingComponent* PawnSensingComp;

	// It handles the delegate FSeePawnDelegate when it was trigged
	UFUNCTION()
	void OnPawnSeen(APawn* Pawn);

};
