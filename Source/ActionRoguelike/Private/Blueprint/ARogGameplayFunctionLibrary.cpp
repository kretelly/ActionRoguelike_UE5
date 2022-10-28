// Fill out your copyright notice in the Description page of Project Settings.


#include "Blueprint/ARogGameplayFunctionLibrary.h"
#include "ARogAttributeComponent.h"
#include "AI/ARogAICharacter.h" 

bool UARogGameplayFunctionLibrary::ApplyDamage(AActor* DamageCauser, AActor* TargetActor, float DamageAmount)
{
	UARogAttributeComponent* AttributeComp = UARogAttributeComponent::GetAttributeComponent(TargetActor);
	if(AttributeComp)
	{
		return AttributeComp->ApplyHealthChange(DamageCauser,-DamageAmount);
	}
	return false;
}

bool UARogGameplayFunctionLibrary::ApplyDirectionalDamage(AActor* DamageCauser, AActor* TargetActor, float DamageAmount, const FHitResult& HitResult)
{
	// Apply Damage
	ApplyDamage(DamageCauser, TargetActor, DamageAmount);

	// Impulse if not alive
	if(!UARogAttributeComponent::IsActorAlive(TargetActor))
	{
		UPrimitiveComponent* PrimitiveComp = HitResult.GetComponent();
		if (PrimitiveComp && PrimitiveComp->IsSimulatingPhysics(HitResult.BoneName))
		{
			// Direction = Target - Origin
			FVector Direction = HitResult.TraceEnd - HitResult.TraceStart;
			Direction.Normalize();

			PrimitiveComp->AddImpulseAtLocation(Direction * 300000.f, HitResult.ImpactPoint, HitResult.BoneName);
		}
		return false; // Already dead
	}
	return true; // Still Alive
}
