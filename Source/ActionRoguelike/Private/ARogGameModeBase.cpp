// Fill out your copyright notice in the Description page of Project Settings.

#include "ARogGameModeBase.h"
#include "EnvironmentQuery/EnvQueryManager.h"
#include "EnvironmentQuery/EnvQuery.h"
#include "EnvironmentQuery/EnvQueryInstanceBlueprintWrapper.h"
#include "AI/ARogAICharacter.h"
#include "ARogAttributeComponent.h"
#include "EngineUtils.h"


AARogGameModeBase::AARogGameModeBase()
{
	SpawnTimerInterval = 3.0f;
}

void AARogGameModeBase::StartPlay()
{
	Super::StartPlay();
	GetWorldTimerManager().SetTimer(SpawnBotTimerHandle, this, &AARogGameModeBase::SpawnBotTimerElapsed, SpawnTimerInterval, true);
}

void AARogGameModeBase::SpawnBotTimerElapsed()
{
	// Count alive bots before spawning
	int32 NrOfAliveBots = 0;
	for (TActorIterator<AARogAICharacter> It(GetWorld()); It; ++It)
	{
		AARogAICharacter* Bot = *It;

		//UARogAttributeComponent* AttributeComp = Cast<UARogAttributeComponent>(Bot->GetComponentByClass(UARogAttributeComponent::StaticClass()));
		//UARogAttributeComponent* AttributeComp = Bot->FindComponentByClass<UARogAttributeComponent>();
		UARogAttributeComponent* AttributeComp = UARogAttributeComponent::GetAttributeComponent(Bot);

		if (ensure(AttributeComp) && AttributeComp->IsAlive())
		{
			NrOfAliveBots++;
		}
	}

	float MaxBotCount = 10.0f;

	// Spawn Curve
	if (DifficultySpawnCurve)
	{
		MaxBotCount = DifficultySpawnCurve->GetFloatValue(GetWorld()->TimeSeconds);
	}

	// Check if it can spawn bots
	if (NrOfAliveBots >= MaxBotCount)
	{
		return;
	}

	UEnvQueryInstanceBlueprintWrapper* QueryInstance = UEnvQueryManager::RunEQSQuery(this, SpawnBotEnvQuery, this, EEnvQueryRunMode::RandomBest5Pct, nullptr);
	if (QueryInstance)
	{
		QueryInstance->GetOnQueryFinishedEvent().AddDynamic(this, &AARogGameModeBase::OnQueryComplete);
	}
}

void AARogGameModeBase::OnQueryComplete(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus)
{
	// Check if it has passed
	if (QueryStatus != EEnvQueryStatus::Success)
	{
		return;
	}

	// Spawn Bot
	TArray<FVector> Locations = QueryInstance->GetResultsAsLocations();
	if (Locations.IsValidIndex(0) && MinionClass)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		
		FVector SpawnLocation = Locations[0];
		SpawnLocation.Z += 120.f;

		GetWorld()->SpawnActor<AActor>(MinionClass, SpawnLocation, FRotator::ZeroRotator, SpawnParams);
	}
}