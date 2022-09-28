// Fill out your copyright notice in the Description page of Project Settings.


#include "ARogCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "ARogInteractionComponent.h"

#include "DrawDebugHelpers.h"

// Sets default values
AARogCharacter::AARogCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	SpringArmComp->SetupAttachment(GetRootComponent());
	SpringArmComp->bUsePawnControlRotation = true;

	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	CameraComp->SetupAttachment(SpringArmComp);

	InteractionComp = CreateDefaultSubobject<UARogInteractionComponent>(TEXT("InteractionComp"));

	GetCharacterMovement()->bOrientRotationToMovement = true;
	bUseControllerRotationYaw = false;
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

	/** This is entirely optional, it draws two arrows to visualize rotations of the player */
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
}

// Called to bind functionality to input
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

// Primary Attack
void AARogCharacter::PrimaryAttack()
{
	if (!GetWorldTimerManager().IsTimerActive(PrimaryAttackTimerHandle))
	{
		PlayAnimMontage(AttackAnim);
		GetWorldTimerManager().SetTimer(PrimaryAttackTimerHandle, this, &AARogCharacter::PrimaryAttackTimeElapsed, 0.2f);
	}
}

void AARogCharacter::PrimaryAttackTimeElapsed()
{
	// Hand Location to Spawn Projectile
	FVector HandLocation = GetMesh()->GetSocketLocation("Muzzle_01");
	// Tranform Spawn
	FTransform SpawnTM = FTransform(GetControlRotation(), HandLocation, FVector(1.0f));
	// Spawn Params
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	if (ProjectileClass)
	{
		GetWorld()->SpawnActor<AActor>(ProjectileClass, SpawnTM, SpawnParams);
	}

	GetWorldTimerManager().ClearTimer(PrimaryAttackTimerHandle);
}

void AARogCharacter::PrimaryInteract()
{
	if (InteractionComp)
	{
		InteractionComp->PrimaryInteract();
	}
}

//GEngine->AddOnScreenDebugMessage(-1, 4.5f, FColor::Purple, "Event Fired");