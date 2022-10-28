// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/ARogAICharacter.h" 	
#include "Perception/PawnSensingComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/CapsuleComponent.h"
#include "ARogAttributeComponent.h"
#include "BrainComponent.h"
#include "AIController.h"
#include "Blueprint/UserWidget.h"
#include "UI/ARogWorldUserWidget.h"
#include "ARogCharacter.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "DrawDebugHelpers.h"


// Sets default values
AARogAICharacter::AARogAICharacter()
{
    PawnSensingComp = CreateDefaultSubobject<UPawnSensingComponent>("PawnSensingComp");

    AttributeComp = CreateDefaultSubobject<UARogAttributeComponent>(TEXT("AttributeComp"));

    //Set collsion
    GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECollisionResponse::ECR_Ignore); //TODO: Create custom collision to handle this.
    GetMesh()->SetGenerateOverlapEvents(true);

    // Set the AI Controller to possess it when placed or spawned into the world
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
    if (Delta <= 0)
    {
        // Create HealthBar on the top of the Minions' head
        if (ActiveHealthBar == nullptr)
        {
            ActiveHealthBar = CreateWidget<UARogWorldUserWidget>(GetWorld(), HealthBarWidgetClass);
            if (ActiveHealthBar)
            {
                ActiveHealthBar->AttachedActor = this;
                ActiveHealthBar->AddToViewport(); // Call widget's construct event
            }
        }

        if (InstigatorActor != this)
        {            
            SetTargetActor(InstigatorActor); 
        }

        if (NewHealth <= 0.0f)
        {
            // Remove HealthBar from viewport
            if (ActiveHealthBar)
            {
                ActiveHealthBar->RemoveFromParent();
            }

            // Stop Behavior Tree
            AAIController* AIController = Cast<AAIController>(GetController());
            if (AIController)
            {
                AIController->GetBrainComponent()->StopLogic("Killed");
            }

            // Ragdoll Collision
            GetMesh()->SetAllBodiesSimulatePhysics(true); //GetMesh()->SetSimulatePhysics(true);
            GetMesh()->SetCollisionProfileName("Ragdoll");
            
            GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
            GetCharacterMovement()->DisableMovement();

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
        // This check avoid it shot another AI Character
        if (Cast<AARogCharacter>(NewTarget))
        {
            //UBlackboardComponent* BlackboardComp = Cast<UBlackboardComponent>(AIController->GetBlackboardComponent());
            //BlackboardComp->SetValueAsObject("TargetActor", NewTarget)
            AIController->GetBlackboardComponent()->SetValueAsObject("TargetActor", NewTarget);
        }
    }
}

