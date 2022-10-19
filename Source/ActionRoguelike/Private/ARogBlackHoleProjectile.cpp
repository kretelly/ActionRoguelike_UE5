// Fill out your copyright notice in the Description page of Project Settings.


#include "ARogBlackHoleProjectile.h"
#include "Components/SphereComponent.h"
#include "PhysicsEngine/RadialForceComponent.h"

// Sets default values
AARogBlackHoleProjectile::AARogBlackHoleProjectile()
{
	// it will only affects if actor that is simulating physics! (e.g. Treasure Chest won't be affects because it is not simulating)

	SphereComp->OnComponentBeginOverlap.AddDynamic(this, &AARogBlackHoleProjectile::OnActorBeginOverlap);
	SphereComp->SetSphereRadius(100.0f);
	SphereComp->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	
	RadialForceComp = CreateDefaultSubobject<URadialForceComponent>("RadialForceComp");
	RadialForceComp->SetupAttachment(SphereComp);
	RadialForceComp->Radius = 1200.0f;
	RadialForceComp->ForceStrength = -2000000.0f;
	RadialForceComp->bIgnoreOwningActor = true;
}

void AARogBlackHoleProjectile::BeginPlay()
{
	Super::BeginPlay();
	SetLifeSpan(5.0f); // Life Span should be called here to avoid the engine crashes
}

void AARogBlackHoleProjectile::OnActorBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherComp->IsSimulatingPhysics())
	{
		OtherActor->Destroy();
	}
}