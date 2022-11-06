// Fill out your copyright notice in the Description page of Project Settings.


#include "ARogMagicProjectile.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Components/ARogAttributeComponent.h"
#include "Blueprint/ARogGameplayFunctionLibrary.h"
#include "Components/ARogActionComponent.h"
#include "Actions/ARogActionEffect.h"

AARogMagicProjectile::AARogMagicProjectile()
{
	SphereComp->OnComponentBeginOverlap.AddDynamic(this, &AARogMagicProjectile::OnActorBeginOverlap);
	Projectile->InitialSpeed = 3000.0f;
	DamageAmount = 25.0f;
}

void AARogMagicProjectile::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	// Lifetime Span
	SetLifeSpan(10.0f);
	// Sphere Radius
	SphereComp->SetSphereRadius(20.0f);
}

void AARogMagicProjectile::OnActorBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor)
	{
		if (OtherActor != GetInstigator())
		{
			// Get and check if the overlaped actor has AttributeComp
			//UARogAttributeComponent* AttributeComp = Cast<UARogAttributeComponent>(OtherActor->GetComponentByClass(UARogAttributeComponent::StaticClass()));
			//UARogAttributeComponent* AttributeComp = OtherActor->FindComponentByClass<UARogAttributeComponent>();
			//UARogAttributeComponent* AttributeComp = UARogAttributeComponent::GetAttributeComponent(OtherActor);

			// Expensive way to get GameplayTag
			//static FGameplayTag ParryTag = FGameplayTag::RequestGameplayTag("Status.Parrying");

			// Parry Attack
			UARogActionComponent* ActionComp = OtherActor->FindComponentByClass<UARogActionComponent>();
			if (ActionComp && ActionComp->ActiveGameplayTags.HasTag(ParryTag))
			{
				Projectile->Velocity = -Projectile->Velocity;
				SetInstigator(Cast<APawn>(OtherActor));
				return;
			}

			if (UARogGameplayFunctionLibrary::ApplyDirectionalDamage(GetInstigator(), OtherActor, DamageAmount, SweepResult))
			{
				Explode();

				// Add Buff
				if (ActionComp)
				{
					ActionComp->AddAction(GetOwner(), BurningActionClass);
				}
			}
		}
	}
}
