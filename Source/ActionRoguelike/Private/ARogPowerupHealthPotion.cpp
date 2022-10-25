// Fill out your copyright notice in the Description page of Project Settings.


#include "ARogPowerupHealthPotion.h"
#include "ARogAttributeComponent.h"

AARogPowerupHealthPotion::AARogPowerupHealthPotion()
{
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>("MeshComp");
	// Disable collision, instead we use SphereComp to handle interaction queries
	MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	MeshComp->SetupAttachment(RootComponent);
}

void AARogPowerupHealthPotion::Interact_Implementation(AActor* InstigatorPawn)
{
	if (!ensure(InstigatorPawn))
	{
		return;
	}

	UARogAttributeComponent* AttributeComp = Cast<UARogAttributeComponent>(InstigatorPawn->GetComponentByClass(UARogAttributeComponent::StaticClass()));
	// Check if not already at max health
	if (ensure(AttributeComp) && !AttributeComp->IsFullHealth())
	{
		// Only activate if healed successfully
		if (AttributeComp->ApplyHealthChange(AttributeComp->GetHealthMax()))
		{
			HideAndCooldownPowerup();
		}
	}
}

