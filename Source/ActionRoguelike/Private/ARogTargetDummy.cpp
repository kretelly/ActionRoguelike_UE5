// Fill out your copyright notice in the Description page of Project Settings.


#include "ARogTargetDummy.h"
#include "Components/StaticMeshComponent.h"
#include "Components/ARogAttributeComponent.h"

// Sets default values
AARogTargetDummy::AARogTargetDummy()
{
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>("MeshComp");
	MeshComp->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
	MeshComp->SetCollisionObjectType(ECollisionChannel::ECC_WorldStatic);
	MeshComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	RootComponent = MeshComp;

	AttributeComp = CreateDefaultSubobject<UARogAttributeComponent>("AttributeComp");
	AttributeComp->ParamChangeDelegate.AddDynamic(this, &AARogTargetDummy::OnParamChange);

	SetReplicates(true); // Allow this actor replicate
}

void AARogTargetDummy::OnParamChange(AActor* InstigatorActor, UARogAttributeComponent* OwningComp, float NewHealth, float Delta)
{
	// Default Values
	// UARogAttributeComponent* AttrComp = Cast<UARogAttributeComponent>(AttributeComp->GetClass()->GetDefaultObject());
	// float DefaultHealth = AttrComp->Health; // The variable should be public

	if (bCanPlayDissolve)
	{
		//float DissolveValue = 1 - (NewHealth/DefaultHealth); // Node: 1-x

		float DefaultHealth = 100.0f;
		float PercentHealth = NewHealth / DefaultHealth;

		if (PercentHealth <= 0) Destroy();

		MeshComp->SetScalarParameterValueOnMaterials("DissolveAmount", PercentHealth);
	}

	if (bCanPlayHitFlash)
	{
		if (NewHealth <= 0) Destroy();
		MeshComp->SetScalarParameterValueOnMaterials("HitFlashTime", GetWorld()->TimeSeconds);
	}
}