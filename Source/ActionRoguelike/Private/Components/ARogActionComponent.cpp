// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/ARogActionComponent.h"
#include "Actions/ARogActionObject.h"
#include "../ActionRoguelike.h"
#include "Net/UnrealNetwork.h"
#include "Engine/ActorChannel.h"

// Sets default values for this component's properties
UARogActionComponent::UARogActionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	// Allow Actor Components be replicated
	SetIsReplicatedByDefault(true);
}

void UARogActionComponent::BeginPlay()
{
	Super::BeginPlay();

	// Start only via server
	if (GetOwner()->HasAuthority())
	{
		// Start player's actions
		for (TSubclassOf<UARogActionObject> ActionClass : DefaultActions)
		{
			AddAction(GetOwner(), ActionClass);
		}
	}
}

void UARogActionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Draw All Actions
 	for (UARogActionObject* Action : Actions)
 	{
		FColor TextColor = Action->IsRunning() ? FColor::Blue : FColor::White;

		FString ActionMsg = FString::Printf(TEXT("[%s] Action: %s, IsRunning: %s, Outer: %s"),
			*GetNameSafe(GetOwner()),
			*Action->ActionName.ToString(),
			Action->IsRunning() ? TEXT("True") : TEXT("False"),
			*GetNameSafe(Action->GetOuter()));

		LogOnScreen(this, ActionMsg, TextColor, 0.0f);
 	}
}

void UARogActionComponent::AddAction(AActor* Instigator, TSubclassOf<UARogActionObject> ActionClass)
{
	if (ActionClass == nullptr)
	{
		return;
	}

	UARogActionObject* NewAction = NewObject<UARogActionObject>(this, ActionClass);
	if (NewAction)
	{
		Actions.Add(NewAction);

		if (NewAction->bAutoStart && NewAction->CanStart(Instigator))
		{
			NewAction->StartAction(Instigator);
		}
	}
}

void UARogActionComponent::RemoveAction(UARogActionObject* ActionToRemove)
{
	if (!ensure(ActionToRemove && !ActionToRemove->IsRunning()))
	{
		return;
	}
	Actions.Remove(ActionToRemove);
}

bool UARogActionComponent::StartActionByName(AActor* Instigator, FName ActionName)
{
	for (UARogActionObject* Action : Actions)
	{
		if (Action && Action->ActionName == ActionName)
		{
			if (!Action->CanStart(Instigator))
			{
				continue;
			}

			// Is Client?
			if (!GetOwner()->HasAuthority())
			{
				ServerStartAction(Instigator, ActionName);
			}

			Action->StartAction(Instigator);
			return true;
		}
	}
	return false;
}

bool UARogActionComponent::StopActionByName(AActor* Instigator, FName ActionName)
{
	for (UARogActionObject* Action : Actions)
	{
		if (Action && Action->ActionName == ActionName)
		{
			if (Action->IsRunning())
			{
				Action->StopAction(Instigator);
				return true;
			}
		}
	}
	return false;
}

bool UARogActionComponent::HasAction(TSubclassOf<UARogActionObject> ActionClass)
{
	for (UARogActionObject* Action : Actions)
	{
		if (Action && Action->IsA(ActionClass))
		{
			return true;
		}
	}
	return false;
}

void UARogActionComponent::ServerStartAction_Implementation(AActor* Instigator, FName ActionName)
{
	StartActionByName(Instigator, ActionName);
}

// This function allow ActorComponent replicate UObjects, it's useful to replicate array and so on.
bool UARogActionComponent::ReplicateSubobjects(UActorChannel* Channel, FOutBunch* Bunch, FReplicationFlags* RepFlags)
{
	bool bWroteSomething = Super::ReplicateSubobjects(Channel, Bunch, RepFlags);
	for (UARogActionObject* Action : Actions)
	{
		if (Action)
		{
			bWroteSomething |= Channel->ReplicateSubobject(Action, *Bunch, *RepFlags);
		}
	}
	return bWroteSomething;
}

void UARogActionComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UARogActionComponent, Actions);
}