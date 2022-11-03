// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/ARogActionComponent.h"
#include "Actions/ARogActionObject.h"

// Sets default values for this component's properties
UARogActionComponent::UARogActionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UARogActionComponent::BeginPlay()
{
	Super::BeginPlay();

	// Start player's actions
	for (TSubclassOf<UARogActionObject> ActionClass : DefaultActions)
	{
		AddAction(GetOwner(), ActionClass);
	}
}

void UARogActionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
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
