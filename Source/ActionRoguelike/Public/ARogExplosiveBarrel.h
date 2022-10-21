// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ARogExplosiveBarrel.generated.h"

class UStaticMeshComponent;
class URadialForceComponent;
class USoundCue;

UCLASS()
class ACTIONROGUELIKE_API AARogExplosiveBarrel : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AARogExplosiveBarrel();

protected:

	// Base Mesh
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* StaticMeshComp;

	// Base Impulse Force
	UPROPERTY(VisibleAnywhere)
	URadialForceComponent* RadialForceComp;

	UPROPERTY(EditDefaultsOnly)
	USoundCue* ImpactSound;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void OnComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

};
