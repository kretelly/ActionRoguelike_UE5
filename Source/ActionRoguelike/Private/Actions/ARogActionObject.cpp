// Fill out your copyright notice in the Description page of Project Settings.


#include "Actions/ARogActionObject.h"
#include "Components/ARogActionComponent.h"


bool UARogActionObject::CanStart_Implementation(AActor* Instigator)
{
	if (IsRunning())
	{
		return false;
	}

	UARogActionComponent* ActionComp = GetOwningComponent();
	if (ActionComp->ActiveGameplayTags.HasAny(BlockedTags))
	{
		return false;
	}

	return true;
}

void UARogActionObject::StartAction_Implementation(AActor* Instigator)
{
	UE_LOG(LogTemp, Log, TEXT("Started: %s"), *GetNameSafe(this));

	UARogActionComponent* ActionComp = GetOwningComponent();
	ActionComp->ActiveGameplayTags.AppendTags(GrantsTags);

	bIsRunning = true;
}

void UARogActionObject::StopAction_Implementation(AActor* Instigator)
{
	UE_LOG(LogTemp, Log, TEXT("Stopped: %s"), *GetNameSafe(this));

	ensureAlways(bIsRunning);

	UARogActionComponent* ActionComp = GetOwningComponent();
	ActionComp->ActiveGameplayTags.RemoveTags(GrantsTags);

	bIsRunning = false;
}

UWorld* UARogActionObject::GetWorld() const
{
	// Outer is set when creating action via NewObject<T>
	UARogActionComponent* ActionComp = Cast<UARogActionComponent>(GetOuter()); // We have setted Outer in Action Component -> AddAction().
	if (ActionComp)
	{
		return ActionComp->GetWorld();
	}
	return nullptr;
}

UARogActionComponent* UARogActionObject::GetOwningComponent() const
{
	return Cast<UARogActionComponent>(GetOuter());
}

bool UARogActionObject::IsRunning() const
{
	return bIsRunning;
}

