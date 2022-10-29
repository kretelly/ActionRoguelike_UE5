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
		AddAction(ActionClass);
	}
}

void UARogActionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UARogActionComponent::AddAction(TSubclassOf<UARogActionObject> ActionClass)
{
	if (ActionClass == nullptr)
	{
		return;
	}

	UARogActionObject* NewAction = NewObject<UARogActionObject>(this, ActionClass);
	if (NewAction)
	{
		Actions.Add(NewAction);
	}
}

bool UARogActionComponent::StartActionByName(AActor* Instigator, FName ActionName)
{
	for (UARogActionObject* Action : Actions)
	{
		if (Action && Action->ActionName == ActionName)
		{
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
			Action->StopAction(Instigator);
			return true;
		}
	}
	return false;
}
