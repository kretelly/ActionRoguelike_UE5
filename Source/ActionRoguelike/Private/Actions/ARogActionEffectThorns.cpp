// Fill out your copyright notice in the Description page of Project Settings.

#include "Actions/ARogActionEffectThorns.h"
#include "Components/ARogAttributeComponent.h"
#include "Components/ARogActionComponent.h"
#include "Blueprint/ARogGameplayFunctionLibrary.h"
#include "Actions/ARogActionObject.h"


// This ability returns back fractions of damage
// For instance, a damage = 20.70f will apply 20.0f to the player and return back 0.70f to the other actor.

UARogActionEffectThorns::UARogActionEffectThorns()
{
	ReflectFraction = 0.2f;

	Duration = 0.0f;
	Period = 0.0f;
}

void UARogActionEffectThorns::StartAction_Implementation(AActor* Instigator)
{
	Super::StartAction_Implementation(Instigator);

	// Start listening
	UARogAttributeComponent* AttributeComp = UARogAttributeComponent::GetAttributeComponent(GetOwningComponent()->GetOwner());
	if (AttributeComp)
	{
		AttributeComp->HealthChangeDelegate.AddDynamic(this, &UARogActionEffectThorns::OnHealthChange);
	}
}

void UARogActionEffectThorns::StopAction_Implementation(AActor* Instigator)
{
	Super::StopAction_Implementation(Instigator);

	// Stop listening
	UARogAttributeComponent* AttributeComp = UARogAttributeComponent::GetAttributeComponent(GetOwningComponent()->GetOwner());
	if (AttributeComp)
	{
		AttributeComp->HealthChangeDelegate.RemoveDynamic(this, &UARogActionEffectThorns::OnHealthChange);
	}
}

void UARogActionEffectThorns::OnHealthChange(AActor* InstigatorActor, UARogAttributeComponent* OwningComp, float NewHealth, float Delta)
{
	AActor* OwningActor = GetOwningComponent()->GetOwner();

	// Damage Only
	if (Delta < 0.0f && OwningActor != InstigatorActor)
	{
		// Round to nearest to avoid 'ugly' damage numbers and tiny reflections
		int32 ReflectedAmount = FMath::RoundToInt(Delta * ReflectFraction);
		if (ReflectedAmount == 0)
		{
			return;
		}

		// Flip to positive, so we don't end up healing ourselves when passed into damage
		ReflectedAmount = FMath::Abs(ReflectedAmount);

		// Return damage sender...
		UARogGameplayFunctionLibrary::ApplyDamage(OwningActor, InstigatorActor, ReflectedAmount);
	}
}