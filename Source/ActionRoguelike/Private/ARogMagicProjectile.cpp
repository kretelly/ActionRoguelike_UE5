// Fill out your copyright notice in the Description page of Project Settings.


#include "ARogMagicProjectile.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Kismet/GameplayStatics.h"
#include "ARogAttributeComponent.h"

// Sets default values
AARogMagicProjectile::AARogMagicProjectile()
{
	SphereComp->OnComponentBeginOverlap.AddDynamic(this, &AARogMagicProjectile::OnActorBeginOverlap);
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
	if (OtherActor && OtherActor != GetInstigator())
	{
		// Get and check if the overlaped actor has AttributeComp
		UARogAttributeComponent* AttributeComp = Cast<UARogAttributeComponent>(OtherActor->GetComponentByClass(UARogAttributeComponent::StaticClass()));
		if (AttributeComp)
		{
			AttributeComp->ApplyHealthChange(-20.0f);
			Explode();
		}
	}
}