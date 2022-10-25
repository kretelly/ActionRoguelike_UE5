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

bool UARogAttributeComponent::ApplyHealthChange(AActor* InstigatorActor, float Delta)
{
	Health += Delta;
	Health = FMath::Clamp(Health, 0.0f, HealthMax);

	// if health is 0, so we should pass a delta equals zero. 
	//if (Health == 0.0f) { Delta = 0.0f; }

	HealthChangeDelegate.Broadcast(InstigatorActor, this, Health, Delta);
	ParamChangeDelegate.Broadcast(InstigatorActor, this, Health, Delta); // Target Dummy

	// Hit Flash -> This code could be added in OnHealthChange Event at our custom ACharacter class.
	ACharacter* Character = Cast<ACharacter>(GetOwner());
	if (Character && UARogAttributeComponent::IsActorAlive(Character))
	{
		FColor Color = FColor::Orange;
		Character->GetMesh()->SetVectorParameterValueOnMaterials("HitFlashColor", FVector(Color));
		Character->GetMesh()->SetScalarParameterValueOnMaterials("HitFlashTime", GetWorld()->TimeSeconds);
	}

	return Health > 0;
}

UARogAttributeComponent* UARogAttributeComponent::GetAttributeComponent(AActor* FromActor)
{
	if (FromActor)
	{
		return FromActor->FindComponentByClass<UARogAttributeComponent>();
		//return Cast<UARogAttributeComponent>(FromActor->GetComponentByClass(UARogAttributeComponent::StaticClass()));
	}

	return nullptr;
}

bool UARogAttributeComponent::IsActorAlive(AActor* Actor)
{
	UARogAttributeComponent* AttributeComp = UARogAttributeComponent::GetAttributeComponent(Actor);

	if (AttributeComp)
	{
		return AttributeComp->IsAlive();
		//return AttributeComp->Health > 0;
	}

	return false;
}
