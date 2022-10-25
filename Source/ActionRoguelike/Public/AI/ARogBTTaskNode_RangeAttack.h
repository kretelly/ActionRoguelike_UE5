// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "ARogBTTaskNode_RangeAttack.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API UARogBTTaskNode_RangeAttack : public UBTTaskNode
{
	GENERATED_BODY()

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

public:
	UARogBTTaskNode_RangeAttack();

protected:
	UPROPERTY(EditAnywhere, Category = AI)
	float MaxBulletSpread;

	UPROPERTY(EditAnywhere, Category = AI)
	TSubclassOf<AActor> MagicProjectileClass;


	
};
