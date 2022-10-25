// Fill out your copyright notice in the Description page of Project Settings.


#include "ARogItemChest.h"
#include "Components/StaticMeshComponent.h"
#include "Particles/ParticleSystemComponent.h"

// Sets default values
AARogItemChest::AARogItemChest()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>("BaseMesh");
	SetRootComponent(BaseMesh);

	LidMesh = CreateDefaultSubobject<UStaticMeshComponent>("LidMesh");
	LidMesh->SetupAttachment(BaseMesh);

	TreasureMesh = CreateDefaultSubobject<UStaticMeshComponent>("TreasureMesh");
	TreasureMesh->SetupAttachment(BaseMesh);

	ParticleSysComp = CreateDefaultSubobject<UParticleSystemComponent>("ParticleSysComp");
	ParticleSysComp->SetupAttachment(BaseMesh);

	TimelineComp = CreateDefaultSubobject<UTimelineComponent>(TEXT("TimelineComp"));
}

// Called when the game starts or when spawned
void AARogItemChest::BeginPlay()
{
	Super::BeginPlay();

	ParticleSysComp->bAutoActivate = false;

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

void AARogItemChest::Interact_Implementation(AActor* InstigatorPawn)
{
	if (!TimelineComp->IsPlaying())
	{
		if (!bIsLidOpen)
		{
			TimelineComp->Play();
			bIsLidOpen = true;
		}	
		else
		{
			TimelineComp->Reverse();
			bIsLidOpen = false;
		}
	}
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

