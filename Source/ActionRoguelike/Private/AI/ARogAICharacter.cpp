// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/ARogAICharacter.h"

// Sets default values
AARogAICharacter::AARogAICharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AARogAICharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AARogAICharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

