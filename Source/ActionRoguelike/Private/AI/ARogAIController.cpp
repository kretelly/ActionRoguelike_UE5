// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/ARogAIController.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"

void AARogAIController::BeginPlay()
{
	Super::BeginPlay();

	// Run our custom BehaviorTree
	RunBehaviorTree(BehaviorTree);

	APawn* TargetPawn = UGameplayStatics::GetPlayerPawn(this, 0);
	if (TargetPawn)
	{
		GetBlackboardComponent()->SetValueAsVector("MoveToLocation", TargetPawn->GetActorLocation());
		GetBlackboardComponent()->SetValueAsObject("TargetActor", TargetPawn);
	}
}