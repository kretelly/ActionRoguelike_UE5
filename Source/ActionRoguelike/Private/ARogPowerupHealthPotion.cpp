// Fill out your copyright notice in the Description page of Project Settings.


#include "ARogPowerupHealthPotion.h"
#include "Components/ARogAttributeComponent.h"
#include "ARogPlayerState.h"

AARogPowerupHealthPotion::AARogPowerupHealthPotion()
{
	CreditCost = 50;
}

void AARogPowerupHealthPotion::Interact_Implementation(APawn* InstigatorPawn)
{
	if (!ensure(InstigatorPawn))
	{
		return;
	}

	//UARogAttributeComponent* AttributeComp = Cast<UARogAttributeComponent>(InstigatorPawn->GetComponentByClass(UARogAttributeComponent::StaticClass()));
	//UARogAttributeComponent* AttributeComp = InstigatorPawn->FindComponentByClass<UARogAttributeComponent>();
	UARogAttributeComponent* AttributeComp = UARogAttributeComponent::GetAttributeComponent(InstigatorPawn);
	
	// Check if not already at max health
	if (ensure(AttributeComp) && !AttributeComp->IsFullHealth())
	{
		if (AARogPlayerState* PS = InstigatorPawn->GetPlayerState<AARogPlayerState>())
		{
			// Only activate if healed successfully
			if (PS->RemoveCredits(CreditCost) && AttributeComp->ApplyHealthChange(this, AttributeComp->GetHealthMax()))
			{
				HideAndCooldownPowerup();
			}
		}
	}
}

