// Fill out your copyright notice in the Description page of Project Settings.


#include "ARogExplosiveBarrel.h"
#include "Components/StaticMeshComponent.h"
#include "PhysicsEngine/RadialForceComponent.h"
#include "Kismet/GameplayStatics.h"
#include "ARogMagicProjectile.h"
#include "Sound/SoundCue.h"

// Sets default values
AARogExplosiveBarrel::AARogExplosiveBarrel()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>("StaticMeshComp");
	StaticMeshComp->SetSimulatePhysics(true);
	StaticMeshComp->SetNotifyRigidBodyCollision(true); // SimulationGeneratesHitEvents
	StaticMeshComp->SetMassOverrideInKg(FName("NAME_None"), 100.0f, true);
	SetRootComponent(StaticMeshComp);

	RadialForceComp = CreateDefaultSubobject<URadialForceComponent>("RadialForceComp");
	RadialForceComp->ImpulseStrength = 200000.0f;
	RadialForceComp->Radius = 700.0f;
	RadialForceComp->bAutoActivate = false;
	RadialForceComp->SetupAttachment(StaticMeshComp);
}

// Called when the game starts or when spawned
void AARogExplosiveBarrel::BeginPlay()
{
	Super::BeginPlay();

	// Hit event
	StaticMeshComp->OnComponentHit.AddDynamic(this, &AARogExplosiveBarrel::OnComponentHit);
}

// Called every frame
void AARogExplosiveBarrel::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called on event OnComponentHit if it was hit by a AARogMagicProjectile actor
void AARogExplosiveBarrel::OnComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	AARogMagicProjectile* Projectile = Cast<AARogMagicProjectile>(OtherActor);

	if (Projectile)
	{
		// Impulse
		RadialForceComp->FireImpulse();

		// Impact Sound
		UGameplayStatics::SpawnSoundAtLocation(GetWorld(), ImpactSound, GetActorLocation());
	}
}
