// Fill out your copyright notice in the Description page of Project Settings.


#include "ARogTargetDummy.h"
#include "Components/StaticMeshComponent.h"
#include "ARogAttributeComponent.h"

// Sets default values
AARogTargetDummy::AARogTargetDummy()
{
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>("MeshComp");
	MeshComp->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	RootComponent = MeshComp;

	AttributeComp = CreateDefaultSubobject<UARogAttributeComponent>("AttributeComp");
	AttributeComp->ParamChangeDelegate.AddDynamic(this, &AARogTargetDummy::OnParamChange);
}

void AARogTargetDummy::OnParamChange(AActor* InstigatorActor, UARogAttributeComponent* OwningComp, float NewHealth, float Delta)
{
	// Default Values
	// UARogAttributeComponent* AttrComp = Cast<UARogAttributeComponent>(AttributeComp->GetClass()->GetDefaultObject());
	// float DefaultHealth = AttrComp->Health; // The variable should be public
	
	float DefaultHealth = 100.0f;

	if (bCanPlayDissolve)
	{
		float DissolveValue = 1 - (NewHealth/DefaultHealth);
		if (DissolveValue > 1) Destroy();

		MeshComp->SetScalarParameterValueOnMaterials("DissolveAmount", DissolveValue);
	}

	if (bCanPlayHitFlash)
	{

		MeshComp->SetScalarParameterValueOnMaterials("HitFlashTime", GetWorld()->TimeSeconds);
	}
}