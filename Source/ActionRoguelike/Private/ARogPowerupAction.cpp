// Fill out your copyright notice in the Description page of Project Settings.


#include "ARogPowerupAction.h"
#include "Actions/ARogActionObject.h"
#include "Components/ARogActionComponent.h"

void AARogPowerupAction::Interact_Implementation(APawn* InstigatorPawn)
{
	// Make sure we have instigator & that action class was set up
	if (!ensure(InstigatorPawn && ActionToGrant))
	{
		return;
	}

	UARogActionComponent* ActionComp = Cast<UARogActionComponent>(InstigatorPawn->GetComponentByClass(UARogActionComponent::StaticClass()));
	if (ActionComp)
	{
		// Check if Player already has action class
		if (ActionComp->HasAction(ActionToGrant))
		{
			//UE_LOG(LogTemp, Log, TEXT("Instigator already has action of class: %s"), *GetNameSafe(ActionToGrant));
			FString DebugMsg = FString::Printf(TEXT("Action '%s' already known."), *GetNameSafe(ActionToGrant));
			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, DebugMsg);
			return;
		}
		// Give new Ability
		ActionComp->AddAction(InstigatorPawn, ActionToGrant);
		HideAndCooldownPowerup();
	}
}