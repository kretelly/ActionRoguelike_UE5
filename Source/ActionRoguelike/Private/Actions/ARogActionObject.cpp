// Fill out your copyright notice in the Description page of Project Settings.


#include "Actions/ARogActionObject.h"
#include "Components/ARogActionComponent.h"

void UARogActionObject::StartAction_Implementation(AActor* Instigator)
{
	UE_LOG(LogTemp, Log, TEXT("Started: %s"), *GetNameSafe(this));
}

void UARogActionObject::StopAction_Implementation(AActor* Instigator)
{
	UE_LOG(LogTemp, Log, TEXT("Stopped: %s"), *GetNameSafe(this));
}

UWorld* UARogActionObject::GetWorld() const
{
	// Outer is set when creating action via NewObject<T>
	UARogActionComponent* ActionComp = Cast<UARogActionComponent>(GetOuter()); // We have setted Outer in this component.
	if (ActionComp)
	{
		return ActionComp->GetWorld();
	}
	return nullptr;
}
