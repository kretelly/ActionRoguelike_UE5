// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GameplayTagContainer.h"
#include "ARogActionObject.generated.h"

class UWorld;
class UARogActionComponent;
/**
 * 
 */
UCLASS(Blueprintable) // Do not forget to add the keyword 'Blueprintable' -> without this we cannot create child classes from this one.
class ACTIONROGUELIKE_API UARogActionObject : public UObject
{
	GENERATED_BODY()

protected:
	
	UFUNCTION(BlueprintCallable, Category = "Action")
	UARogActionComponent* GetOwningComponent() const;

	/* Tags added to owning actor when activated, removed when action stops */
	UPROPERTY(EditDefaultsOnly, Category = "Tags")
	FGameplayTagContainer GrantsTags;

	/* Action can only start if OwningActor has none of these Tags applied */
	UPROPERTY(EditDefaultsOnly, Category = "Tags")
	FGameplayTagContainer BlockedTags;

	// Track if we are running any attack / ability
	bool bIsRunning;

public:

	/* Start immediately when added to an action component */
	UPROPERTY(EditDefaultsOnly, Category = "Action")
	bool bAutoStart;

	UFUNCTION(BlueprintCallable, Category = "Action")
	bool IsRunning() const;

	UFUNCTION(BlueprintNativeEvent, Category = "Action")
	bool CanStart(AActor* Instigator);

	UFUNCTION(BlueprintNativeEvent, Category = "Action")
	void StartAction(AActor* Instigator);

	UFUNCTION(BlueprintNativeEvent, Category = "Action")
	void StopAction(AActor* Instigator);
	
	/* Action nickname to start/stop without a reference to the object */
	UPROPERTY(EditDefaultsOnly, Category = "Action")
	FName ActionName;

	// We have to override this function to be possible to call it and also its functions from Blueprint, since this class is a UObject.
	UWorld* GetWorld() const override;

};
