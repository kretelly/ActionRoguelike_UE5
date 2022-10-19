// Fill out your copyright notice in the Description page of Project Settings.


#include "ARogAttributeComponent.h"

// Sets default values for this component's properties
UARogAttributeComponent::UARogAttributeComponent()
{
	Health = 100.0f;
	HealthMax = 100.0f;
}

bool UARogAttributeComponent::IsAlive() const
{
	return Health > 0.0f;
}

bool UARogAttributeComponent::ApplyHealthChange(float Delta)
{
	Health += Delta;

	HealthChangeDelegate.Broadcast(nullptr, this, Health, Delta);
	ParamChangeDelegate.Broadcast(nullptr, this, Health, Delta);

	return Health > 0;
}
