// Fill out your copyright notice in the Description page of Project Settings.


#include "ARogCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"

// Sets default values
AARogCharacter::AARogCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	SpringArmComp->SetupAttachment(GetRootComponent());
	
	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	CameraComp->SetupAttachment(SpringArmComp);
	
}

// Called when the game starts or when spawned
void AARogCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AARogCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AARogCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	if(PlayerInputComponent)
	{
		PlayerInputComponent->BindAxis("MoveForward",this, &AARogCharacter::MoveForward);
		PlayerInputComponent->BindAxis("Turn",this, &APawn::AddControllerYawInput);
	}
}

// Move Character
void AARogCharacter::MoveForward(float Value)
{
	AddMovementInput(GetActorForwardVector(), Value);
}