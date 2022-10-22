// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/ARogAIController.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"

void AARogAIController::BeginPlay()
{
	Super::BeginPlay();

	if (ensureMsgf(BehaviorTree, TEXT("Behavior Tree is nullptr! Please assign BehaviorTree in your AI Controller.")))
	{
		// Run our custom BehaviorTree
		RunBehaviorTree(BehaviorTree);
	}
	
	/** Example of how set blackboard's variables from AIController
	
	APawn* TargetPawn = UGameplayStatics::GetPlayerPawn(this, 0);
	if (TargetPawn)
	{
		GetBlackboardComponent()->SetValueAsVector("MoveToLocation", TargetPawn->GetActorLocation());
		GetBlackboardComponent()->SetValueAsObject("TargetActor", TargetPawn);
	}
	*/
}