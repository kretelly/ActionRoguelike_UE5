// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ARogTargetDummy.generated.h"

class UStaticMeshComponent;
class UARogAttributeComponent;

UCLASS()
class ACTIONROGUELIKE_API AARogTargetDummy : public AActor
{
	GENERATED_BODY()
	
public:
	AARogTargetDummy();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Options, meta = (EditCondition = "!bCanPlayHitFlash"))
	bool bCanPlayDissolve;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Options, meta = (EditCondition = "!bCanPlayDissolve"))
	bool bCanPlayHitFlash;

protected:

	UPROPERTY(VisibleAnywhere)
	UARogAttributeComponent* AttributeComp;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* MeshComp;

	UFUNCTION()
	void OnParamChange(AActor* InstigatorActor, UARogAttributeComponent* OwningComp, float NewHealth, float Delta);
};
