// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/ARogActionComponent.h"
#include "Actions/ARogActionObject.h"
#include "../ActionRoguelike.h"
#include "Net/UnrealNetwork.h"
#include "Engine/ActorChannel.h"

// Declaring my own custom 'stat' command, DECLARE_CYCLE_STAT(CounterName,StatId,GroupId) 
DECLARE_CYCLE_STAT(TEXT("StartActionByName"), STAT_StartActionByName, STATGROUP_ACTIONROGUELIKE); // Using my custom stat group -> my custom group is defined in 'ActionRoguelike.h' 
//DECLARE_CYCLE_STAT(TEXT("StartActionByName"), STAT_StartActionByName, STATGROUP_StatSystem); // Using Default group

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

void UARogActionComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	// Stop all
	TArray<UARogActionObject*> ActionsCopy = Actions;
	for (UARogActionObject* Action : ActionsCopy)
	{
		if (Action && Action->IsRunning())
		{
			Action->StopAction(GetOwner());
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

		//FString ActionMsg = FString::Printf(TEXT("[%s] Action: %s, IsRunning: %s, Outer: %s"),*GetNameSafe(GetOwner()),*Action->ActionName.ToString(),Action->IsRunning() ? TEXT("True") : TEXT("False"),*GetNameSafe(Action->GetOuter()));
		FString ActionMsg = FString::Printf(TEXT("[%s] Action: %s"), *GetNameSafe(GetOwner()), *GetNameSafe(Action));

		LogOnScreen(this, ActionMsg, TextColor, 0.0f);
 	}
}

void UARogActionComponent::AddAction(AActor* Instigator, TSubclassOf<UARogActionObject> ActionClass)
{
	if (ActionClass == nullptr)
	{
		return;
	}

	// Only server can add action
	if(!GetOwner()->HasAuthority())
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
	// Defining a scope for our Custom Command
	SCOPE_CYCLE_COUNTER(STAT_StartActionByName);

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

			/* Using curly brackets we define an environment where our custom command should track
			{
				SCOPE_CYCLE_COUNTER(STAT_StartActionByName);
				Action->StartAction(Instigator);
			}
			*/

			// Bookmark for Unreal Insights
			TRACE_BOOKMARK(TEXT("StartAction::%s"), *GetNameSafe(Action));

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
				// Is Client?
				if (!GetOwner()->HasAuthority())
				{
					ServerStopAction(Instigator, ActionName);
				}

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

void UARogActionComponent::ServerStopAction_Implementation(AActor* Instigator, FName ActionName)
{
	StopActionByName(Instigator, ActionName);
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