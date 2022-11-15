// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ARogMonsterDataAsset.generated.h"

class UARogActionObject;

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API UARogMonsterDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:

	/* I could use and TArray of Monsters, so I could creat data assets separed by monster classification (e.g. Ranged, Melee, and so on) */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Spawn Info")
	TSubclassOf<AActor> MonsterClass;

	/* Actions/buffs to grant this Monster */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Spawn Info")
	TArray<TSubclassOf<UARogActionObject>> Actions;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
	UTexture2D* Icon;

	FPrimaryAssetId GetPrimaryAssetId() const override
	{
		return FPrimaryAssetId("Monsters", GetFName());
	}
	
};
