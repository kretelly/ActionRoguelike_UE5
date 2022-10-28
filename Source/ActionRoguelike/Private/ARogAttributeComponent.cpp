// Fill out your copyright notice in the Description page of Project Settings.


#include "ARogAttributeComponent.h"
#include "GameFramework/Character.h"
#include "ARogGameModeBase.h"


// Console variable to control increase or reduce the damage
static TAutoConsoleVariable<float> CVarDamageMultiplier(TEXT("ARog.CVarDamageMultiplier"), true, TEXT("Global Damage Modifier for Attribute Component."), EConsoleVariableFlags::ECVF_Cheat);


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

float UARogAttributeComponent::GetHealth() const
{
	return Health;
}

float UARogAttributeComponent::GetHealthMax() const
{
	return HealthMax;
}

bool UARogAttributeComponent::ApplyHealthChange(AActor* InstigatorActor, float Delta)
{
	// god mode
	if (!GetOwner()->CanBeDamaged() && Delta < 0.0f)
	{
		return false;
	}

	// Calling CVarDamageMultiplier console variable
	// The great about this cvar is the fact I control the damage, so I can teste many number until I feel the great damage amount.
	if (Delta < 0.0f)
	{
		float DamageMultiplier = CVarDamageMultiplier.GetValueOnGameThread();
		Delta *= DamageMultiplier;
	}

	Health = FMath::Clamp(Health + Delta, 0.0f, HealthMax);

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

	// Died -> this handles respawn!
	if (Health <= 0.0f)
	{
		AARogGameModeBase* GM = GetWorld()->GetAuthGameMode<AARogGameModeBase>();
		if(GM)
		{
			GM->OnActorKilled(GetOwner(), InstigatorActor);
		}
	}

	return Health > 0; //True if still alive, otherwise false.
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

bool UARogAttributeComponent::Kill(AActor* InstigatorActor)
{
	return ApplyHealthChange(InstigatorActor, -GetHealthMax());
}
