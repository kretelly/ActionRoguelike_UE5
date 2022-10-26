// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ARogAICharacter.generated.h"

class UPawnSensingComponent;
class UARogAttributeComponent;
class UARogWorldUserWidget;

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

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UARogAttributeComponent* AttributeComp;

	// It handles the delegate FSeePawnDelegate when it was trigged
	UFUNCTION()
	void OnPawnSeen(APawn* Pawn);

	UFUNCTION()
	void OnHealthChange(AActor* InstigatorActor, UARogAttributeComponent* OwningComp, float NewHealth, float Delta);

	void SetTargetActor(AActor* NewTarget);

	UARogWorldUserWidget* ActiveHealthBar;

	UPROPERTY(EditDefaultsOnly, Category = UI)
	TSubclassOf<UARogWorldUserWidget> HealthBarWidgetClass;

};
