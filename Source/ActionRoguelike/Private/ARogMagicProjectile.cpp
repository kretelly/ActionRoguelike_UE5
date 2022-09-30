// Fill out your copyright notice in the Description page of Project Settings.


#include "ARogMagicProjectile.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AARogMagicProjectile::AARogMagicProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SphereComp = CreateDefaultSubobject<USphereComponent>("SphereComp");
	//SphereComp->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	//SphereComp->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	//SphereComp->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	//SphereComp->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Ignore);
	//SphereComp->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldStatic, ECollisionResponse::ECR_Block);
	SphereComp->SetCollisionProfileName("Projectile");
	SetRootComponent(SphereComp);

	ParticleSysComp = CreateDefaultSubobject<UParticleSystemComponent>("ParticleSysComp");
	ParticleSysComp->SetupAttachment(SphereComp);

	Projectile = CreateDefaultSubobject<UProjectileMovementComponent>("Projectile");
	Projectile->InitialSpeed = 2000.0f;
	Projectile->bRotationFollowsVelocity = true;
	Projectile->bInitialVelocityInLocalSpace = true;
	Projectile->ProjectileGravityScale = 0.0f;
}

// Called when the game starts or when spawned
void AARogMagicProjectile::BeginPlay()
{
	Super::BeginPlay();

	// Lifetime Span
	SetLifeSpan(10.0f);

	// Hit event
	SphereComp->OnComponentHit.AddDynamic(this, &AARogMagicProjectile::OnComponentHit);
	
	//Ignores the intigator that spawned the projectile.
	SphereComp->IgnoreActorWhenMoving(GetInstigator(),true);
}

// Called every frame
void AARogMagicProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called on event OnComponentHit if it was hit by a AARogMagicProjectile actor
void AARogMagicProjectile::OnComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor != GetInstigator())
	{
		if (HitParticle)
		{
			FTransform SpawnTransform = FTransform(Hit.Location);
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitParticle, SpawnTransform);
		}

		//Add play sound here
		GEngine->AddOnScreenDebugMessage(-1, 2.5f, FColor::Blue, "Fire Sound");

		Destroy();
	}
}
