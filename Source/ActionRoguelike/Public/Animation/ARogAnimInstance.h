// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "ARogAnimInstance.generated.h"

class UARogActionComponent;
/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API UARogAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

protected:

	/* Is Pawn Stunned based on GameplayTag data */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation")
	bool bIsStunned;

	UPROPERTY(BlueprintReadOnly, Category = "Animation")
	UARogActionComponent* ActionComp;

	// Similar Begin Play()
	void NativeInitializeAnimation() override;

	// Similar Tick()
	void NativeUpdateAnimation(float DeltaSeconds) override;
	
};
