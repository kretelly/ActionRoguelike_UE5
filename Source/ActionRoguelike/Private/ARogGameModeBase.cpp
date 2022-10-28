// Fill out your copyright notice in the Description page of Project Settings.

#include "ARogGameModeBase.h"
#include "EnvironmentQuery/EnvQueryManager.h"
#include "EnvironmentQuery/EnvQuery.h"
#include "EnvironmentQuery/EnvQueryInstanceBlueprintWrapper.h"
#include "AI/ARogAICharacter.h"
#include "ARogAttributeComponent.h"
#include "ARogCharacter.h"
#include "EngineUtils.h"


// Creating our console variable to control the spawn bot
static TAutoConsoleVariable<bool> CVarSpawnBots(TEXT("ARog.SpawnBots"), true, TEXT("Toggle spawn bots rate!"), EConsoleVariableFlags::ECVF_Cheat);

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
	// Calling our console variable
	if (!CVarSpawnBots.GetValueOnGameThread())
	{
		UE_LOG(LogTemp, Warning, TEXT("Bot spawning disable via cvar 'CVarSpawnBots'"));
		return;
	}

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

void AARogGameModeBase::KillAll()
{
	for (TActorIterator<AARogAICharacter> It(GetWorld()); It; ++It)
	{
		AARogAICharacter* Bot = *It;

		UARogAttributeComponent* AttributeComp = UARogAttributeComponent::GetAttributeComponent(Bot);

		if (ensure(AttributeComp) && AttributeComp->IsAlive())
		{
			AttributeComp->Kill(this); // TODO: call from player fom kill credits
		}
	}

}

void AARogGameModeBase::RespawnPlayerElapsed(AController* Controller)
{
	if (Controller)
	{
		Controller->UnPossess();
		RestartPlayer(Controller);
	}
}

void AARogGameModeBase::OnActorKilled(AActor* VictimActor, AActor* KillerActor)
{
	AARogCharacter* Player = Cast<AARogCharacter>(VictimActor);
	if (Player)
	{
		FTimerHandle RespawnPlayerTimerHandle;

		FTimerDelegate Delegate;
		Delegate.BindUObject(this, &AARogGameModeBase::RespawnPlayerElapsed, Player->GetController());

		float RespawnDelay = 2.0f;
		GetWorld()->GetTimerManager().SetTimer(RespawnPlayerTimerHandle, Delegate, RespawnDelay, false);
	}
}




