// Fill out your copyright notice in the Description page of Project Settings.


#include "ARogCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/ARogInteractionComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/ARogAttributeComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Components/CapsuleComponent.h"
#include "Components/ARogActionComponent.h"

#include "DrawDebugHelpers.h"

// Sets default values
AARogCharacter::AARogCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	SpringArmComp->SetupAttachment(GetRootComponent());
	SpringArmComp->bUsePawnControlRotation = true;
	SpringArmComp->SocketOffset = FVector(0.0f, 90.0f, 0.0f);

	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	CameraComp->SetupAttachment(SpringArmComp);

	InteractionComp = CreateDefaultSubobject<UARogInteractionComponent>(TEXT("InteractionComp"));

	AttributeComp = CreateDefaultSubobject<UARogAttributeComponent>(TEXT("AttributeComp"));
	
	ActionComp = CreateDefaultSubobject<UARogActionComponent>(TEXT("ActionComp"));

	GetCharacterMovement()->bOrientRotationToMovement = true;
	bUseControllerRotationYaw = false;
}

void AARogCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	AttributeComp->HealthChangeDelegate.AddDynamic(this, &AARogCharacter::OnHealthChange);
}

void AARogCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void AARogCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	/** This is entirely optional, it draws two arrows to visualize rotations of the player
	
	// -- Rotation Visualization -- //
	const float DrawScale = 100.0f;
	const float Thickness = 5.0f;
	FVector LineStart = GetActorLocation();
	// Offset to the right of pawn
	LineStart += GetActorRightVector() * 100.0f;
	// Set line end in direction of the actor's forward
	FVector ActorDirection_LineEnd = LineStart + (GetActorForwardVector() * 100.0f);
	// Draw Actor's Direction
	DrawDebugDirectionalArrow(GetWorld(), LineStart, ActorDirection_LineEnd, DrawScale, FColor::Yellow, false, 0.0f, 0, Thickness);
	FVector ControllerDirection_LineEnd = LineStart + (GetControlRotation().Vector() * 100.0f);
	// Draw 'Controller' Rotation ('PlayerController' that 'possessed' this character)
	DrawDebugDirectionalArrow(GetWorld(), LineStart, ControllerDirection_LineEnd, DrawScale, FColor::Green, false, 0.0f, 0, Thickness);
	
	*/
}

void AARogCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	if(PlayerInputComponent)
	{
		// Movement
		PlayerInputComponent->BindAxis("MoveForward",this, &AARogCharacter::MoveForward);
		PlayerInputComponent->BindAxis("MoveRight",this, &AARogCharacter::MoveRight);		
		PlayerInputComponent->BindAxis("Turn",this, &APawn::AddControllerYawInput);
		PlayerInputComponent->BindAxis("LookUp",this, &APawn::AddControllerPitchInput);

		// Jump
		PlayerInputComponent->BindAction("Jump", EInputEvent::IE_Pressed, this, &ACharacter::Jump);

		// Actions
		PlayerInputComponent->BindAction("PrimaryAttack", EInputEvent::IE_Pressed, this, &AARogCharacter::PrimaryAttack);
		PlayerInputComponent->BindAction("PrimaryInteract", EInputEvent::IE_Pressed, this, &AARogCharacter::PrimaryInteract);
		PlayerInputComponent->BindAction("Teleport", EInputEvent::IE_Pressed, this, &AARogCharacter::TeleportAbility);
		PlayerInputComponent->BindAction("SecundaryAttack", EInputEvent::IE_Pressed, this, &AARogCharacter::SecundaryAttack);
		PlayerInputComponent->BindAction("Sprint", EInputEvent::IE_Pressed, this, &AARogCharacter::SprintStart);
		PlayerInputComponent->BindAction("Sprint", EInputEvent::IE_Released, this, &AARogCharacter::SprintStop);
		PlayerInputComponent->BindAction("Parry", EInputEvent::IE_Pressed, this, &AARogCharacter::ToggleParry);
	}
}

// Move Character
void AARogCharacter::MoveForward(float Value)
{
	FRotator ControlRot = GetControlRotation();
	ControlRot.Pitch = 0.0f;
	ControlRot.Roll = 0.0f;
	AddMovementInput(ControlRot.Vector(), Value);
}

void AARogCharacter::MoveRight(float Value)
{
	FRotator ControlRot = GetControlRotation();
	ControlRot.Pitch = 0.0f;
	ControlRot.Roll = 0.0f;

	const FVector Direction = FRotationMatrix(ControlRot).GetUnitAxis(EAxis::Y);
	AddMovementInput(Direction, Value);
}

void AARogCharacter::SprintStart()
{
	ActionComp->StartActionByName(this, "Sprint");
}

void AARogCharacter::SprintStop()
{
	ActionComp->StopActionByName(this, "Sprint");
}

// Primary Interact (Interfaces)
void AARogCharacter::PrimaryInteract()
{
	if (InteractionComp)
	{
		InteractionComp->PrimaryInteract();
	}
}

// Primary Attack
void AARogCharacter::PrimaryAttack()
{
	ActionComp->StartActionByName(this, "PrimaryAttack");
}

// Secundary Attack 
void AARogCharacter::SecundaryAttack()
{
	ActionComp->StartActionByName(this, "SecundaryAttack"); // Black Hole
}

// Teleport
void AARogCharacter::TeleportAbility()
{
	ActionComp->StartActionByName(this, "TeleportAbility");
}

// Parry Defense
void AARogCharacter::ToggleParry()
{
	if (!ActionComp->ActiveGameplayTags.HasTag(ParryTag))
	{
		ActionComp->ActiveGameplayTags.AddTag(ParryTag);
	}
	else
	{
		ActionComp->ActiveGameplayTags.RemoveTag(ParryTag);
	}
}

void AARogCharacter::OnHealthChange(AActor* InstigatorActor, UARogAttributeComponent* OwningComp, float NewHealth, float Delta)
{
	if (NewHealth <= 0)
	{		
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision); // Avoid get stuck on dead pawn and also avoid multiplies spawn.

		APlayerController* PC = Cast<APlayerController>(GetController());
		DisableInput(PC);
	}
}

void AARogCharacter::HealtSelf(float Amount /* = 100.0f */)
{
	AttributeComp->ApplyHealthChange(this, Amount);
}

FVector AARogCharacter::GetPawnViewLocation() const
{
	return CameraComp->GetComponentLocation();
}

//GEngine->AddOnScreenDebugMessage(-1, 4.5f, FColor::Purple, "Event Fired");
