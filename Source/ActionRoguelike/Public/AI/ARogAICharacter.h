// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ARogAICharacter.generated.h"

class UPawnSensingComponent;
class UARogAttributeComponent;
class UARogWorldUserWidget;
class UARogActionComponent;

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

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UARogActionComponent* ActionComp;

	// It handles the delegate FSeePawnDelegate when it was trigged
	UFUNCTION()
	void OnPawnSeen(APawn* Pawn);

	UFUNCTION(NetMulticast, Unreliable)
	void MulticastPawnSeen();

	UFUNCTION()
	void OnHealthChange(AActor* InstigatorActor, UARogAttributeComponent* OwningComp, float NewHealth, float Delta);

	void SetTargetActor(AActor* NewTarget);
	
	AActor* GetTargetActor() const;

	UARogWorldUserWidget* ActiveHealthBar;
	
	/* Widget to display when bot first sees a player. */
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> SpottedWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category = UI)
	TSubclassOf<UARogWorldUserWidget> HealthBarWidgetClass;

};
