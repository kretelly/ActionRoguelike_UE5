// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/ARogBTTaskNode_HealSelf.h"
#include "ARogAttributeComponent.h"
#include "AIController.h"

EBTNodeResult::Type UARogBTTaskNode_HealSelf::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	APawn* AIPawn = Cast<APawn>(OwnerComp.GetAIOwner()->GetPawn());
	if (AIPawn == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	UARogAttributeComponent* AttributeComp = UARogAttributeComponent::GetAttributeComponent(AIPawn);
	if (ensure(AttributeComp))
	{
		AttributeComp->ApplyHealthChange(AIPawn, AttributeComp->GetHealthMax());
	}

	return EBTNodeResult::Succeeded;
}
