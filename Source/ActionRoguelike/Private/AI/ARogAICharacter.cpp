// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/ARogAICharacter.h" 	
#include "Perception/PawnSensingComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "ARogAttributeComponent.h"
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
}

void AARogAICharacter::OnPawnSeen(APawn* Pawn)
{
    AAIController* AIController = Cast<AAIController>(GetController());
    
    if (AIController)
    {
        UBlackboardComponent* BlackboardComp = Cast<UBlackboardComponent>(AIController->GetBlackboardComponent());
        BlackboardComp->SetValueAsObject("TargetActor", Pawn);

        DrawDebugString(GetWorld(), GetActorLocation(), "PLAYER SPOTTED", nullptr, FColor::White, 4.0f, true);
    }
}