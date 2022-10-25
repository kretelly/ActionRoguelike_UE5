// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ARogAttributeComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnHealthChange, AActor*, InstigatorActor, UARogAttributeComponent*, OwningComp, float, NewHealth, float, Delta);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ACTIONROGUELIKE_API UARogAttributeComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UARogAttributeComponent();

protected:

	// EditAnywhere - edit in BP editor and per-instance in level.
	// VisibleAnywhere - 'read-only' in editor and level. (Use for Components)
	// EditDefaultsOnly - hide variable per-instance, edit in BP editor only
	// VisibleDefaultsOnly - 'read-only' access for variable, only in BP editor (uncommon)
	// EditInstanceOnly - allow only editing of instance (eg. when placed in level)
	// --
	// BlueprintReadOnly - read-only in the Blueprint scripting (does not affect 'details'-panel)
	// BlueprintReadWrite - read-write access in Blueprints
	// --
	// Category = "" - display only for detail panels and blueprint context menu.
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Attibutes)
	float Health;

	UPROPERTY(BlueprintReadOnly, Category = Attibutes)
	float HealthMax;

public:

	UPROPERTY(BlueprintAssignable)
	FOnHealthChange HealthChangeDelegate;

	UFUNCTION(BlueprintCallable)
	bool IsAlive() const; // Add the const make the BlueprintCallable pure function.

	UFUNCTION(BlueprintCallable)
	bool IsFullHealth() const;

	UFUNCTION(BlueprintCallable)
	float GetHealthMax() const;

	UFUNCTION(BlueprintCallable)
	bool ApplyHealthChange(float Delta);

	// Dummy Actor -> used to change scalar parameters using event dispatcher
	FOnHealthChange ParamChangeDelegate;


//protected:
	// Called when the game starts
	//virtual void BeginPlay() override;

//public:
	// Called every frame
	//virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
		
};
