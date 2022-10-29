// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/ARogInteractionComponent.h"
#include "ARogGameplayInterface.h"

static TAutoConsoleVariable<bool> CVarDebugDrawInteraction(TEXT("ARog.InteractionDebugDraw"), false, TEXT("Enable Debug Lines for Interact Component."), ECVF_Cheat);

void UARogInteractionComponent::PrimaryInteract()
{
	// Cvar
	bool bDebugDraw = CVarDebugDrawInteraction.GetValueOnGameThread();

	// Interaction
	FCollisionObjectQueryParams ObjectQueryParams;
	ObjectQueryParams.AddObjectTypesToQuery(ECollisionChannel::ECC_WorldDynamic);

	AActor* Owner = GetOwner();
	FVector EyeLocation;
	FRotator EyeRotation;
	Owner->GetActorEyesViewPoint(EyeLocation, EyeRotation);
	FVector End = EyeLocation + (EyeRotation.Vector() * 1000.0f);

	/**	Simple Line Trace
	
	FHitResult HitResult;
	bool bIsBlockingLine = GetWorld()->LineTraceSingleByObjectType(HitResult, EyeLocation, End, ObjectQueryParams);

	AActor* HitActor = HitResult.GetActor();
	if (HitActor)
	{
		// Check if the hitted actor implents the our custom Interface
		if (HitActor->Implements<UARogGameplayInterface>())
		{
			APawn* Pawn = Cast<APawn>(Owner);
			if (Pawn)
			{
				IARogGameplayInterface::Execute_Interact(HitActor, Pawn);
			}
		}
	}

	FColor LineColor = bIsBlockingLine ? FColor::Green : FColor::Red;
	DrawDebugLine(GetWorld(), EyeLocation, End, LineColor, false, 2.0f, 0, 2.0f);

	*/

	// Sweep
	TArray<FHitResult> Hits;

	float Radius = 17.f;	
	FCollisionShape Shape;
	Shape.SetSphere(Radius);

	bool bIsBlockingLine = GetWorld()->SweepMultiByObjectType(Hits, EyeLocation, End, FQuat::Identity, ObjectQueryParams, Shape);
	FColor LineColor = bIsBlockingLine ? FColor::Green : FColor::Red;

	for (auto Hit : Hits)
	{
		AActor* HitActor = Hit.GetActor();
		if (HitActor)
		{
			// Check if the hitted actor implents the our custom Interface
			if (HitActor->Implements<UARogGameplayInterface>())
			{
				APawn* Pawn = Cast<APawn>(Owner);
				if (Pawn)
				{
					IARogGameplayInterface::Execute_Interact(HitActor, Pawn);

					if(bDebugDraw)	DrawDebugSphere(GetWorld(), Hit.ImpactPoint, Radius, 32, LineColor, false, 2.0f, 0, 1.f);

					break; // Break the for after found the first interact object
				}
			}
		}
	}

	if (bDebugDraw)	DrawDebugLine(GetWorld(), EyeLocation, End, LineColor, false, 2.0f, 0, 2.0f);

}
