// Fill out your copyright notice in the Description page of Project Settings.


#include "ARogPowerupCredits.h"
#include "ARogPlayerState.h"

AARogPowerupCredits::AARogPowerupCredits()
{
	CreditsAmount = 80;
}

void AARogPowerupCredits::Interact_Implementation(APawn* InstigatorPawn)
{
	if (!ensure(InstigatorPawn))
	{
		return;
	}

	if (AARogPlayerState* PS = InstigatorPawn->GetPlayerState<AARogPlayerState>())
	{
		PS->AddCredits(CreditsAmount);
		HideAndCooldownPowerup();
	}
}
