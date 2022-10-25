// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/ARogAICharacter.h" 	
#include "Perception/PawnSensingComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/CapsuleComponent.h"
#include "ARogAttributeComponent.h"
#include "BrainComponent.h"
#include "AIController.h"

#include "DrawDebugHelpers.h"


// Sets default values
AARogAICharacter::AARogAICharacter()
{
    PawnSensingComp = CreateDefaultSubobject<UPawnSensingComponent>("PawnSensingComp");

    AttributeComp = CreateDefaultSubobject<UARogAttributeComponent>(TEXT("AttributeComp"));

    // Set the AI Controller to posses it
    AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

void AARogAICharacter::PostInitializeComponents()
{
    Super::PostInitializeComponents();
    
    PawnSensingComp->OnSeePawn.AddDynamic(this, &AARogAICharacter::OnPawnSeen);

    AttributeComp->HealthChangeDelegate.AddDynamic(this, &AARogAICharacter::OnHealthChange);
}

void AARogAICharacter::OnHealthChange(AActor* InstigatorActor, UARogAttributeComponent* OwningComp, float NewHealth, float Delta)
{
    //if (NewHealth <= 0 && Delta <= 0.0f)

    if (NewHealth <= 0)
    {
        if (InstigatorActor != this)
        {
            // TODO: Check if the Instigador is not an AARogAICharacter!
            SetTargetActor(InstigatorActor);
        }

        if (Delta <= 0.0f)
        {
            // Stop Behavior Tree
            AAIController* AIController = Cast<AAIController>(GetController());
            if (AIController)
            {
                AIController->GetBrainComponent()->StopLogic("Killed");
            }

            // Ragdoll Collision
            //GetMesh()->SetSimulatePhysics(true);
            GetMesh()->SetAllBodiesSimulatePhysics(true);
            GetMesh()->SetCollisionProfileName("Ragdoll");
            GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
            //GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

            // Set Life Span
            SetLifeSpan(7.0f);
        }
    }
}

void AARogAICharacter::OnPawnSeen(APawn* Pawn)
{
    SetTargetActor(Pawn);
    DrawDebugString(GetWorld(), GetActorLocation(), "PLAYER SPOTTED", nullptr, FColor::White, 4.0f, true);
}

void AARogAICharacter::SetTargetActor(AActor* NewTarget)
{
    AAIController* AIController = Cast<AAIController>(GetController());
    if (AIController)
    {
        //UBlackboardComponent* BlackboardComp = Cast<UBlackboardComponent>(AIController->GetBlackboardComponent());
        //BlackboardComp->SetValueAsObject("TargetActor", NewTarget);

        AIController->GetBlackboardComponent()->SetValueAsObject("TargetActor", NewTarget);
    }
}

