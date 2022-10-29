// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/ARogBTService_CheckHealth.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/ARogAttributeComponent.h"
#include "AIController.h"

UARogBTService_CheckHealth::UARogBTService_CheckHealth()
{
	LowHealthFraction = 0.3f;
}

void UARogBTService_CheckHealth::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	APawn* AIPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (ensure(AIPawn))
	{
		UARogAttributeComponent* AttributeComp = UARogAttributeComponent::GetAttributeComponent(AIPawn);
		if (ensure(AttributeComp))
		{
			bool bLowHealth = (AttributeComp->GetHealth() / AttributeComp->GetHealthMax()) < LowHealthFraction;

			UBlackboardComponent* BlackBoardComp = OwnerComp.GetBlackboardComponent();
			BlackBoardComp->SetValueAsBool(LowHealthKey.SelectedKeyName, bLowHealth);			
		}
	}
}
