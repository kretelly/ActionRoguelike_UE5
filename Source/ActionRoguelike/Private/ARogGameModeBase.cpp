// Fill out your copyright notice in the Description page of Project Settings.

#include "ARogGameModeBase.h"
#include "EnvironmentQuery/EnvQueryManager.h"
#include "EnvironmentQuery/EnvQuery.h"
#include "EnvironmentQuery/EnvQueryInstanceBlueprintWrapper.h"
#include "AI/ARogAICharacter.h"
#include "Components/ARogAttributeComponent.h"
#include "ARogPlayerState.h"
#include "ARogCharacter.h"
#include "EngineUtils.h"
#include "Kismet/GameplayStatics.h"
#include "Save/ARogSaveGame.h"
#include "GameFramework/GameStateBase.h"
#include "ARogGameplayInterface.h"
#include "Serialization/ObjectAndNameAsStringProxyArchive.h"
#include "Engine/AssetManager.h"
#include "Data/ARogMonsterDataAsset.h"
#include "Components/ARogActionComponent.h"
#include "Subsystem/ARogSaveGameSubsystem.h"

// Creating our console variable to control the spawn bot
static TAutoConsoleVariable<bool> CVarSpawnBots(TEXT("ARog.SpawnBots"), true, TEXT("Toggle spawn bots rate!"), EConsoleVariableFlags::ECVF_Cheat);

AARogGameModeBase::AARogGameModeBase()
{
	SpawnTimerInterval = 2.0f;
	CreditsPerKill = 20;

	DesiredPowerupCount = 10;
	RequiredPowerupDistance = 2000;

	PlayerStateClass = AARogPlayerState::StaticClass();

	SlotName = "DefaultSaveGame";
}

void AARogGameModeBase::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);

	/** NOTE:
	* The load transform will not property work in Actor with bIsSpatiallyLoaded setted as true
	* because it may not be available when the InitGame() is called in level with 'World Partition' enabled
	* One way to handle that or work around is use the postload functions of our actor and implement some logic.
	*
	* https://courses.tomlooman.com/courses/1320807/lectures/35211646/comments/16776540
	*/

	// (Save/Load logic moved into new SaveGameSubsystem)
	UARogSaveGameSubsystem* SG = GetGameInstance()->GetSubsystem<UARogSaveGameSubsystem>();

	// Optional slot name (Falls back to slot specified in SaveGameSettings class/INI otherwise)
	FString SelectedSaveSlot = UGameplayStatics::ParseOption(Options, "SaveGame");
	SG->LoadSaveGame(SelectedSaveSlot);
}

void AARogGameModeBase::StartPlay()
{
	Super::StartPlay();

	// Continuous timer to spawn in more bots.
	// Actual amount of bots and whether its allowed to spawn determined by spawn logic later in the chain...
	GetWorldTimerManager().SetTimer(SpawnBotTimerHandle, this, &AARogGameModeBase::SpawnBotTimerElapsed, SpawnTimerInterval, true);

	// Make sure we have assigned at least one power-up class
	if (ensure(PowerupClasses.Num() > 0))
	{
		// Run EQS to find potential power-up spawn locations
		UEnvQueryInstanceBlueprintWrapper* QueryInstance = UEnvQueryManager::RunEQSQuery(this, PowerupSpawnQuery, this, EEnvQueryRunMode::AllMatching, nullptr);
		if (ensure(QueryInstance))
		{
			QueryInstance->GetOnQueryFinishedEvent().AddDynamic(this, &AARogGameModeBase::OnPowerupSpawnQueryCompleted);
		}
	}
}

// This function it is very helpful when it comes to multiplayer
void AARogGameModeBase::HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer)
{

	// Calling Before Super:: so we set variables before 'beginplayingstate' is called in PlayerController (which is where we instantiate UI)
	UARogSaveGameSubsystem* SG = GetGameInstance()->GetSubsystem<UARogSaveGameSubsystem>();
	SG->HandleStartingNewPlayer(NewPlayer);

	Super::HandleStartingNewPlayer_Implementation(NewPlayer);

	// Now we're ready to override spawn location
	// Alternatively we could override core spawn location to use store locations immediately (skipping the whole 'find player start' logic)
	SG->OverrideSpawnTransform(NewPlayer);
}

void AARogGameModeBase::KillAll()
{
	for (TActorIterator<AARogAICharacter> It(GetWorld()); It; ++It)
	{
		AARogAICharacter* Bot = *It;

		UARogAttributeComponent* AttributeComp = UARogAttributeComponent::GetAttributeComponent(Bot);
		if (ensure(AttributeComp) && AttributeComp->IsAlive())
		{
			AttributeComp->Kill(this); // @fixme: pass in player? for kill credit
		}
	}
}

void AARogGameModeBase::SpawnBotTimerElapsed()
{
	if (!CVarSpawnBots.GetValueOnGameThread())
	{
		UE_LOG(LogTemp, Warning, TEXT("Bot spawning disabled via cvar 'CVarSpawnBots'."));
		return;
	}

	int32 NrOfAliveBots = 0;
	for (TActorIterator<AARogAICharacter> It(GetWorld()); It; ++It)
	{
		AARogAICharacter* Bot = *It;

		UARogAttributeComponent* AttributeComp = UARogAttributeComponent::GetAttributeComponent(Bot);
		if (ensure(AttributeComp) && AttributeComp->IsAlive())
		{
			NrOfAliveBots++;
		}
	}

	UE_LOG(LogTemp, Log, TEXT("Found %i alive bots."), NrOfAliveBots);

	float MaxBotCount = 10.0f;
	if (DifficultySpawnCurve)
	{
		MaxBotCount = DifficultySpawnCurve->GetFloatValue(GetWorld()->TimeSeconds);
	}

	if (NrOfAliveBots >= MaxBotCount)
	{
		UE_LOG(LogTemp, Log, TEXT("At maximum bot capacity. Skipping bot spawn."));
		return;
	}

	UEnvQueryInstanceBlueprintWrapper* QueryInstance = UEnvQueryManager::RunEQSQuery(this, SpawnBotQuery, this, EEnvQueryRunMode::RandomBest5Pct, nullptr);
	if (ensure(QueryInstance))
	{
		QueryInstance->GetOnQueryFinishedEvent().AddDynamic(this, &AARogGameModeBase::OnBotSpawnQueryCompleted);
	}
}

void AARogGameModeBase::OnBotSpawnQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus)
{
	if (QueryStatus != EEnvQueryStatus::Success)
	{
		UE_LOG(LogTemp, Warning, TEXT("Spawn bot EQS Query Failed!"));
		return;
	}

	TArray<FVector> Locations = QueryInstance->GetResultsAsLocations();
	if (Locations.IsValidIndex(0))
	{
		if (MonsterTable)
		{
			TArray<FMonsterInfoRow*> Rows;
			MonsterTable->GetAllRows("", Rows);

			// Get Random Enemy
			int32 RandomIndex = FMath::RandRange(0, Rows.Num() - 1);
			FMonsterInfoRow* SelectedRow = Rows[RandomIndex];

			UAssetManager* Manager = UAssetManager::GetIfValid();
			if (Manager)
			{
				TArray<FName> Bundles;
				FStreamableDelegate Delegate = FStreamableDelegate::CreateUObject(this, &AARogGameModeBase::OnMonsterLoaded, SelectedRow->MonsterId, Locations[0]);
				Manager->LoadPrimaryAsset(SelectedRow->MonsterId, Bundles, Delegate);
			}
		}
	}
}

void AARogGameModeBase::OnMonsterLoaded(FPrimaryAssetId LoadedId, FVector SpawnLocation)
{
	UAssetManager* Manager = UAssetManager::GetIfValid();
	if (Manager)
	{
		UARogMonsterDataAsset* MonsterData = Cast<UARogMonsterDataAsset>(Manager->GetPrimaryAssetObject(LoadedId));
		if (MonsterData)
		{
			AActor* Actor = GetWorld()->SpawnActor<AActor>(MonsterData->MonsterClass, SpawnLocation, FRotator::ZeroRotator);
			if (Actor)
			{
				UARogActionComponent* ActionComp = Cast<UARogActionComponent>(Actor->FindComponentByClass<UARogActionComponent>());
				if (ActionComp)
				{
					// Grants actions, buffs, and so on during spawn.
					for (TSubclassOf<UARogActionObject> ActionClass : MonsterData->Actions)
					{
						ActionComp->AddAction(Actor, ActionClass);
					}
				}
			}
		}
	}
}

void AARogGameModeBase::OnPowerupSpawnQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus)
{
	if (QueryStatus != EEnvQueryStatus::Success)
	{
		UE_LOG(LogTemp, Warning, TEXT("Spawn bot EQS Query Failed!"));
		return;
	}

	TArray<FVector> Locations = QueryInstance->GetResultsAsLocations();

	// Keep used locations to easily check distance between points
	TArray<FVector> UsedLocations;

	int32 SpawnCounter = 0;
	// Break out if we reached the desired count or if we have no more potential positions remaining
	while (SpawnCounter < DesiredPowerupCount && Locations.Num() > 0)
	{
		// Pick a random location from remaining points.
		int32 RandomLocationIndex = FMath::RandRange(0, Locations.Num() - 1);

		FVector PickedLocation = Locations[RandomLocationIndex];
		// Remove to avoid picking again
		Locations.RemoveAt(RandomLocationIndex);

		// Check minimum distance requirement
		bool bValidLocation = true;
		for (FVector OtherLocation : UsedLocations)
		{
			float DistanceTo = (PickedLocation - OtherLocation).Size();

			if (DistanceTo < RequiredPowerupDistance)
			{
				// Show skipped locations due to distance
				//DrawDebugSphere(GetWorld(), PickedLocation, 50.0f, 20, FColor::Red, false, 10.0f);

				// too close, skip to next attempt
				bValidLocation = false;
				break;
			}
		}

		// Failed the distance test
		if (!bValidLocation)
		{
			continue;
		}

		// Pick a random powerup-class
		int32 RandomClassIndex = FMath::RandRange(0, PowerupClasses.Num() - 1);
		TSubclassOf<AActor> RandomPowerupClass = PowerupClasses[RandomClassIndex];

		GetWorld()->SpawnActor<AActor>(RandomPowerupClass, PickedLocation, FRotator::ZeroRotator);

		// Keep for distance checks
		UsedLocations.Add(PickedLocation);
		SpawnCounter++;
	}
}

void AARogGameModeBase::RespawnPlayerElapsed(AController* Controller)
{
	if (ensure(Controller))
	{
		Controller->UnPossess();
		RestartPlayer(Controller);
	}
}

void AARogGameModeBase::OnActorKilled(AActor* VictimActor, AActor* Killer)
{
	UE_LOG(LogTemp, Log, TEXT("OnActorKilled: Victim: %s, Killer: %s"), *GetNameSafe(VictimActor), *GetNameSafe(Killer));

	// Respawn Players after delay
	AARogCharacter* Player = Cast<AARogCharacter>(VictimActor);
	if (Player)
	{
		// Store time if it was better than previous record
		AARogPlayerState* PS = Player->GetPlayerState<AARogPlayerState>();
		if (PS)
		{
			PS->UpdatePersonalRecord(GetWorld()->TimeSeconds);
		}
		UARogSaveGameSubsystem* SG = GetGameInstance()->GetSubsystem<UARogSaveGameSubsystem>();
		// Immediately auto save on death
		SG->WriteSaveGame();
	}

	// Give Credits for kill (AI does not have Splayer State)
	APawn* KillerPawn = Cast<APawn>(Killer);
	if (KillerPawn)
	{
		AARogPlayerState* PS = KillerPawn->GetPlayerState<AARogPlayerState>();
		if (PS)
		{
			PS->AddCredits(CreditsPerKill);
		}
	}
}
