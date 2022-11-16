// Fill out your copyright notice in the Description page of Project Settings.


#include "ARogItemChest.h"
#include "Components/StaticMeshComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Net/UnrealNetwork.h"

// Sets default values
AARogItemChest::AARogItemChest()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>("BaseMesh");
	BaseMesh->SetSimulatePhysics(true);
	SetRootComponent(BaseMesh);

	LidMesh = CreateDefaultSubobject<UStaticMeshComponent>("LidMesh");
	LidMesh->SetupAttachment(BaseMesh);

	TreasureMesh = CreateDefaultSubobject<UStaticMeshComponent>("TreasureMesh");
	TreasureMesh->SetupAttachment(BaseMesh);

	ParticleSysComp = CreateDefaultSubobject<UParticleSystemComponent>("ParticleSysComp");
	ParticleSysComp->SetupAttachment(BaseMesh);
	ParticleSysComp->bAutoActivate = false;

	TimelineComp = CreateDefaultSubobject<UTimelineComponent>(TEXT("TimelineComp"));

	// World Partition
	bIsSpatiallyLoaded = false;
	//SetIsSpatiallyLoaded(false);

	bReplicates = true; // Allow this actor replicate
	//SetReplicates(true);
	SetReplicateMovement(true); // Save Game Replicates
	BaseMesh->SetIsReplicated(true); // Save Game Replicates
}

// Called when the game starts or when spawned
void AARogItemChest::BeginPlay()
{
	Super::BeginPlay();

	// Timeline track
	FOnTimelineFloat TimelineCurveTrack;
	TimelineCurveTrack.BindDynamic(this, &AARogItemChest::ToggleChestLid);
	if (AnimationCurveFloat) TimelineComp->AddInterpFloat(AnimationCurveFloat, TimelineCurveTrack);

	// Timeline track end
	FOnTimelineEventStatic TimelineCurveFinished;
	TimelineCurveFinished.BindUFunction(this, FName("OnEventFinish"));
	TimelineComp->SetTimelineFinishedFunc(TimelineCurveFinished);

	// Make timeline stop at last frame
	TimelineComp->SetTimelineLengthMode(ETimelineLengthMode::TL_LastKeyFrame);
}

void AARogItemChest::Interact_Implementation(APawn* InstigatorPawn)
{
	bIsLidOpen = !bIsLidOpen; // Every time this value change the OnRep_LidOpened() will be called on Client.
	OnRep_LidOpened(); // Calling RepNotify on Server.
}

void AARogItemChest::OnActorLoaded_Implementation()
{
	OnRep_LidOpened();
}

void AARogItemChest::ToggleChestLid(float Output)
{
	LidMesh->SetRelativeRotation(FRotator(Output, 0.0f, 0.0f));
}

void AARogItemChest::OnEventFinish()
{
	if (bIsLidOpen)
	{
		ParticleSysComp->Activate();
	}
	GEngine->AddOnScreenDebugMessage(-1, 2.5f, FColor::Red, "Fire Sound");
}

// Everytime we replicate any varibale we should override this function
// There is no need to specify this function in our header file, because it is included in our .generated.h file
void AARogItemChest::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	// Args -> (Class, Variable)
	DOREPLIFETIME(AARogItemChest, bIsLidOpen);
}

void AARogItemChest::OnRep_LidOpened()
{
	if (!TimelineComp->IsPlaying())
	{
		if (bIsLidOpen)
		{
			TimelineComp->Play();
		}
		else
		{
			TimelineComp->Reverse();
		}
	}
}
