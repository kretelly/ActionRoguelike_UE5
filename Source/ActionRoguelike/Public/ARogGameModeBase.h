// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "ARogGameModeBase.generated.h"

class UEnvQuery;
class UEnvQueryInstanceBlueprintWrapper;
class UCurveFloat;

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API AARogGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	AARogGameModeBase();

	virtual void StartPlay() override;

protected:

	FTimerHandle SpawnBotTimerHandle;

	UPROPERTY(EditDefaultsOnly, Category = AI)
	float SpawnTimerInterval;

	UPROPERTY(EditDefaultsOnly, Category = AI)
	UCurveFloat* DifficultySpawnCurve;

	UFUNCTION()
	void SpawnBotTimerElapsed();

	UPROPERTY(EditDefaultsOnly, Category = AI)
	TSubclassOf<AActor> MinionClass;

	UPROPERTY(EditDefaultsOnly, Category = AI)
	UEnvQuery* SpawnBotEnvQuery;

	UFUNCTION()
	void OnQueryComplete(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus);

public:

	UFUNCTION(Exec)
	void KillAll();

};
