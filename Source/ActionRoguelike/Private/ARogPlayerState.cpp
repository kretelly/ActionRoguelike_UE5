// Fill out your copyright notice in the Description page of Project Settings.

#include "ARogPlayerState.h"
#include "Save/ARogSaveGame.h"
#include "Net/UnrealNetwork.h"

void AARogPlayerState::AddCredits(int32 Delta)
{
	// Avoid user-error of adding a negative amount or zero
	if (!(Delta > 0.0f))
	{
		return;
	}

	Credits += Delta;

	OnCreditsChanged.Broadcast(this, Credits, Delta);
}

bool AARogPlayerState::RemoveCredits(int32 Delta)
{
	// Avoid user-error of adding a subtracting negative amount or zero
	if (!ensure(Delta > 0.0f))
	{
		return false;
	}

	if (Credits < Delta)
	{
		// Not enough credits available
		return false;
	}

	Credits -= Delta;

	OnCreditsChanged.Broadcast(this, Credits, -Delta);

	return true;
}

void AARogPlayerState::SavePlayerState_Implementation(UARogSaveGame* SaveObject)
{
	if (SaveObject)
	{
		SaveObject->Credits = Credits;
	}
}

void AARogPlayerState::LoadPlayerState_Implementation(UARogSaveGame* SaveObject)
{
	if (SaveObject)
	{
		//Credits = SaveObject->Credits;
		
		// Using this function we trigger the dispatcher event
		AddCredits(SaveObject->Credits);
	}
}

void AARogPlayerState::OnRep_Credits(int32 OldCredits)
{
	OnCreditsChanged.Broadcast(this, Credits, Credits - OldCredits);
}

int32 AARogPlayerState::GetCredits() const
{
	return Credits;
}

void AARogPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AARogPlayerState, Credits);
}