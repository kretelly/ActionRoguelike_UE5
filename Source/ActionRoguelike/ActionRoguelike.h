// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/** 
* Full list of Macros available as there are some additional useful ways to track your code.
* Engine\Source\Runtime\Core\Public\Stats\Stats.h
* 
* Full list of Groups available and much more
* Engine\Source\Runtime\Core\Public\Stats\Stats2.h 
*/

// Declaring my own stat group, this define new "stat command" group shown in-game via "stat ActionRoguelike"
DECLARE_STATS_GROUP(TEXT("ActionRoguelike"), STATGROUP_ACTIONROGUELIKE, STATCAT_Advanced);


static void LogOnScreen(UObject* WorldContext, FString Msg, FColor Color = FColor::White, float Duration = 5.0f)
{
	if (!ensure(WorldContext))
	{
		return;
	}

	UWorld* World = WorldContext->GetWorld();
	if (!ensure(World))
	{
		return;
	}

	FString NetPrefix = World->IsNetMode(ENetMode::NM_Client) ? "[CLIENT] " : "[SERVER] ";
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, Duration, Color, NetPrefix + Msg);
	}
}