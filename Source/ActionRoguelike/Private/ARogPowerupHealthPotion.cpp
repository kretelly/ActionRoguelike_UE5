// Fill out your copyright notice in the Description page of Project Settings.


#include "ARogPowerupHealthPotion.h"
#include "Components/ARogAttributeComponent.h"
#include "ARogPlayerState.h"

#define LOCTEXT_NAMESPACE "InteractableActors"

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

FText AARogPowerupHealthPotion::GetInteractText_Implementation(APawn* InstigatorPawn)
{
	UARogAttributeComponent* AttributeComp = UARogAttributeComponent::GetAttributeComponent(InstigatorPawn);
	if (AttributeComp && AttributeComp->IsFullHealth())
	{
		// If I do not define the LOCTEXT_NAMESPACE I have to use the NSLOCTEXT() and set it manualy
		// return NSLOCTEXT("Namespace", "UniqueKey", "Text");
		// return NSLOCTEXT("InteractableActors", "HealthPotion_FullHealthWarning", "Already at full health.");		
		return LOCTEXT("HealthPotion_FullHealthWarning", "Already at full health.");
	}

	// If I do not define the LOCTEXT_NAMESPACE I have to use the NSLOCTEXT() and set it manualy
	// return FText::Format(NSLOCTEXT("InteractableActors", "HealthPotion_InteractMessage", "Cost {0} Credits. Restores health to maximum."), CreditCost);
	return FText::Format(LOCTEXT("HealthPotion_InteractMessage", "Cost {0} Credits. Restores health to maximum."), CreditCost);
}

#undef LOCTEXT_NAMESPACE
