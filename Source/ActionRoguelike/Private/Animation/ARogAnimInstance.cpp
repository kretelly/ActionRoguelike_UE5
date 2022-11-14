// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/ARogAnimInstance.h"
#include "Components/ARogActionComponent.h"
#include "GameplayTagContainer.h"

void UARogAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	AActor* OwningActor = GetOwningActor();
	if (OwningActor)
	{
		ActionComp = Cast<UARogActionComponent>(OwningActor->GetComponentByClass(UARogActionComponent::StaticClass()));
	}
}

void UARogAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	static FGameplayTag StunnedTag = FGameplayTag::RequestGameplayTag("Status.Stunned");
	if (ActionComp)
	{
		bIsStunned = ActionComp->ActiveGameplayTags.HasTag(StunnedTag);
	}
}
