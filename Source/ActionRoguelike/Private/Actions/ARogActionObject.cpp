// Fill out your copyright notice in the Description page of Project Settings.


#include "Actions/ARogActionObject.h"
#include "Components/ARogActionComponent.h"
#include "Net/UnrealNetwork.h"
#include "../ActionRoguelike.h"

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
	//LogOnScreen(this, FString::Printf(TEXT("Started: %s"), *ActionName.ToString()), FColor::Green);

	UARogActionComponent* ActionComp = GetOwningComponent();
	ActionComp->ActiveGameplayTags.AppendTags(GrantsTags);

	RepData.bIsRunning = true;
	RepData.Instigator = Instigator;
}

void UARogActionObject::StopAction_Implementation(AActor* Instigator)
{
	UE_LOG(LogTemp, Log, TEXT("Stopped: %s"), *GetNameSafe(this));
	//LogOnScreen(this, FString::Printf(TEXT("Stopped: %s"), *ActionName.ToString()), FColor::White);

	ensureAlways(RepData.bIsRunning);

	UARogActionComponent* ActionComp = GetOwningComponent();
	ActionComp->ActiveGameplayTags.RemoveTags(GrantsTags);

	RepData.bIsRunning = false;
	RepData.Instigator = Instigator;
}

UWorld* UARogActionObject::GetWorld() const
{
	// Outer is set when creating action via NewObject<T>
	// AActor* Actor = Cast<AActor>(GetOuter()); -> It could be an AActor for instance if I specify on New UObject declaration.
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
	return RepData.bIsRunning;
}

void UARogActionObject::OnRep_RepData()
{
	if (RepData.bIsRunning)
	{
		StartAction(RepData.Instigator);
	}
	else
	{
		StopAction(RepData.Instigator);
	}
}

void UARogActionObject::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UARogActionObject, RepData);
}