// Fill out your copyright notice in the Description page of Project Settings.


#include "ARogAttributeComponent.h"
#include "GameFramework/Character.h"


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

bool  UARogAttributeComponent::IsFullHealth() const
{
	return Health == HealthMax;
}

float UARogAttributeComponent::GetHealthMax() const
{
	return HealthMax;
}

bool UARogAttributeComponent::ApplyHealthChange(float Delta)
{
	Health += Delta;
	Health = FMath::Clamp(Health, 0.0f, HealthMax);

	HealthChangeDelegate.Broadcast(nullptr, this, Health, Delta);
	ParamChangeDelegate.Broadcast(nullptr, this, Health, Delta);

	// Hit Flash
	ACharacter* Character = Cast<ACharacter>(GetOwner());
	if (Character)
	{
		FColor Color = FColor::Orange;
		Character->GetMesh()->SetVectorParameterValueOnMaterials("HitFlashColor", FVector(Color));
		Character->GetMesh()->SetScalarParameterValueOnMaterials("HitFlashTime", GetWorld()->TimeSeconds);
	}	

	return Health > 0;
}
