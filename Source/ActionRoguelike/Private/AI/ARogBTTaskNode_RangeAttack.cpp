// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/ARogBTTaskNode_RangeAttack.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Character.h"
#include "ARogAttributeComponent.h"
#include "AIController.h"


UARogBTTaskNode_RangeAttack::UARogBTTaskNode_RangeAttack()
{
	MaxBulletSpread = 3.0f;
}

EBTNodeResult::Type UARogBTTaskNode_RangeAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIController = OwnerComp.GetAIOwner();
	
	if (AIController)
	{
		ACharacter* AIPawn = Cast<ACharacter>(AIController->GetPawn());
		if (AIPawn == nullptr)
		{
			return EBTNodeResult::Failed;
		}

		AActor* TargetActor = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject("TargetActor"));
		if (TargetActor == nullptr)
		{
			return EBTNodeResult::Failed;
		}

		if(!UARogAttributeComponent::IsActorAlive(TargetActor))
		{ 
			return EBTNodeResult::Failed;
		}

		// Socket location to spawn Magic Projectile shot
		FVector MuzzleLocation = AIPawn->GetMesh()->GetSocketLocation("Muzzle_01");

		FVector Direction = TargetActor->GetActorLocation() - MuzzleLocation;
		FRotator MuzzleRotation = Direction.Rotation();
		
		// Rand Pitch and Yaw to make the game more fun
		MuzzleRotation.Pitch += FMath::RandRange(0.0f, MaxBulletSpread); // it ignores negative pitch to not hit the floor in front itself
		MuzzleRotation.Yaw += FMath::RandRange(-MaxBulletSpread, MaxBulletSpread);

		FActorSpawnParameters Params;
		Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		Params.Instigator = AIPawn;

		AActor* Projectile = GetWorld()->SpawnActor<AActor>(MagicProjectileClass, MuzzleLocation, MuzzleRotation, Params);

		return Projectile ? EBTNodeResult::Succeeded : EBTNodeResult::Failed;
	}

	return EBTNodeResult::Failed;
}
