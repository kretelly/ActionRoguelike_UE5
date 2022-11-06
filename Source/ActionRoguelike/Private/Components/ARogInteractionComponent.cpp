// Fill out your copyright notice in the Description page of Project Settings.

#include "Components/ARogInteractionComponent.h"
#include "ARogGameplayInterface.h"
#include "UI/ARogWorldUserWidget.h"

static TAutoConsoleVariable<bool> CVarDebugDrawInteraction(TEXT("ARog.InteractionDebugDraw"), false, TEXT("Enable Debug Lines for Interact Component."), ECVF_Cheat);

UARogInteractionComponent::UARogInteractionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	
	TraceRadius = 30.0f;
	TraceDistance = 500.0f;
	CollisionChannel = ECollisionChannel::ECC_WorldDynamic;
}

void UARogInteractionComponent::BeginPlay()
{
	Super::BeginPlay();
}

// Tick runs at client and server
void UARogInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// This function will only be called locally by the owner, only the responsable by the input will call this function.
	// Avoid it be called every time by everyone in a multiplayer through RPC.
	APawn* Pawn = Cast<APawn>(GetOwner());
	if (Pawn->IsLocallyControlled())
	{
		FindBestInteractable();
	}
}

void UARogInteractionComponent::FindBestInteractable()
{
	// Cvar
	bool bDebugDraw = CVarDebugDrawInteraction.GetValueOnGameThread();

	FCollisionObjectQueryParams ObjectQueryParams;
	ObjectQueryParams.AddObjectTypesToQuery(CollisionChannel);

	AActor* Owner = GetOwner();

	FVector EyeLocation;
	FRotator EyeRotation;
	Owner->GetActorEyesViewPoint(EyeLocation, EyeRotation);
	FVector End = EyeLocation + (EyeRotation.Vector() * TraceDistance);

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
	if (bDebugDraw)	DrawDebugLine(GetWorld(), EyeLocation, End, LineColor, false, 2.0f, 0, 2.0f);
	*/

	// Sweep
	TArray<FHitResult> Hits;

	FCollisionShape Shape;
	Shape.SetSphere(TraceRadius);

	bool bBlockingHit = GetWorld()->SweepMultiByObjectType(Hits, EyeLocation, End, FQuat::Identity, ObjectQueryParams, Shape);

	FColor LineColor = bBlockingHit ? FColor::Green : FColor::Red;

	// Clear ref before trying to fill
	FocusedActor = nullptr;

	for (FHitResult Hit : Hits)
	{
		AActor* HitActor = Hit.GetActor();
		if (HitActor)
		{
			// Check if the hitted actor implents our custom Interface
			if (HitActor->Implements<UARogGameplayInterface>())
			{
				FocusedActor = HitActor;
				
				if (bDebugDraw)
				{
					DrawDebugSphere(GetWorld(), Hit.ImpactPoint, TraceRadius, 32, LineColor, false, 2.0f, 0, 1.f);
				}

				break;
			}
		}
	}

	if (FocusedActor)
	{
		if (DefaultWidgetInstance == nullptr && ensure(DefaultWidgetClass))
		{
			DefaultWidgetInstance = CreateWidget<UARogWorldUserWidget>(GetWorld(), DefaultWidgetClass);
		}

		if (DefaultWidgetInstance)
		{
			DefaultWidgetInstance->AttachedActor = FocusedActor;

			if (!DefaultWidgetInstance->IsInViewport())
			{
				DefaultWidgetInstance->AddToViewport();
			}
		}
	}
	else
	{
		if (DefaultWidgetInstance)
		{
			DefaultWidgetInstance->RemoveFromParent();
		}
	}

	if (bDebugDraw) DrawDebugLine(GetWorld(), EyeLocation, End, LineColor, false, 2.0f, 0, 2.0f);
}

void UARogInteractionComponent::PrimaryInteract()
{
	ServerInteract(FocusedActor);
}

void UARogInteractionComponent::ServerInteract_Implementation(AActor* InFocus)
{
	if (InFocus == nullptr)
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, "No Focus Actor to interact.");
		return;
	}

	APawn* Pawn = Cast<APawn>(GetOwner());
	IARogGameplayInterface::Execute_Interact(InFocus, Pawn);
}