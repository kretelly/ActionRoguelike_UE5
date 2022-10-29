// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ARogActionObject.generated.h"

class UWorld;
/**
 * 
 */
UCLASS(Blueprintable) // Do not forget to add the keyword 'Blueprintable' -> without this we cannot create child classes from this one.
class ACTIONROGUELIKE_API UARogActionObject : public UObject
{
	GENERATED_BODY()

public:

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
