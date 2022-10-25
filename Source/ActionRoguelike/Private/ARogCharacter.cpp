// Fill out your copyright notice in the Description page of Project Settings.


#include "ARogCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "ARogInteractionComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "ARogAttributeComponent.h"
#include "Kismet/GameplayStatics.h"

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

	GetCharacterMovement()->bOrientRotationToMovement = true;
	bUseControllerRotationYaw = false;

	HandSocketName = "Muzzle_01";
}

void AARogCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	AttributeComp->HealthChangeDelegate.AddDynamic(this, &AARogCharacter::OnHealthChange);
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
		PlayerInputComponent->BindAction("Teleport", EInputEvent::IE_Pressed, this, &AARogCharacter::TeleportAbility);
		PlayerInputComponent->BindAction("SecundaryAttack", EInputEvent::IE_Pressed, this, &AARogCharacter::SecundaryAttack);
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

void AARogCharacter::StartAttackEffects()
{
	PlayAnimMontage(AttackAnim);
	UGameplayStatics::SpawnEmitterAttached(CastingEffect, GetMesh(), HandSocketName, FVector::ZeroVector, FRotator::ZeroRotator, EAttachLocation::SnapToTarget);
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
	if (!GetWorldTimerManager().IsTimerActive(PrimaryAttackTimerHandle))
	{
		StartAttackEffects();

		FTimerDelegate TimerDelegate;
		TimerDelegate.BindUObject(this, &AARogCharacter::AbilityTimerElapsed, MagicProjectileClass);
		GetWorldTimerManager().SetTimer(PrimaryAttackTimerHandle, TimerDelegate, 0.2f, false);
	}
}

// Secundary Attack
void AARogCharacter::SecundaryAttack()
{
	if (!GetWorldTimerManager().IsTimerActive(SecundaryAttackTimerHandle))
	{
		StartAttackEffects();

		FTimerDelegate TimerDelegate;
		TimerDelegate.BindUObject(this, &AARogCharacter::AbilityTimerElapsed, BlackHoleProjectileClass);
		GetWorldTimerManager().SetTimer(SecundaryAttackTimerHandle, TimerDelegate, 0.2f, false);
	}
}

// Teleport
void AARogCharacter::TeleportAbility()
{
	if (!GetWorldTimerManager().IsTimerActive(TeleportTimerHandle))
	{
		StartAttackEffects();

		FTimerDelegate TimerDelegate;
		TimerDelegate.BindUObject(this, &AARogCharacter::AbilityTimerElapsed, TeleportProjectileClass);
		GetWorldTimerManager().SetTimer(TeleportTimerHandle, TimerDelegate, 0.2f, false);
	}
}

void AARogCharacter::AbilityTimerElapsed(TSubclassOf<AActor> Object)
{
	// Hand Location to Spawn Projectile
	FVector HandLocation = GetMesh()->GetSocketLocation("Muzzle_01");

	// Target Location (using linetrace)
	FCollisionObjectQueryParams ObjectQueryParams;
	ObjectQueryParams.AddObjectTypesToQuery(ECollisionChannel::ECC_WorldStatic);
	ObjectQueryParams.AddObjectTypesToQuery(ECollisionChannel::ECC_WorldDynamic);
	ObjectQueryParams.AddObjectTypesToQuery(ECollisionChannel::ECC_PhysicsBody);

	FVector StartLocation = CameraComp->GetComponentLocation();
	FVector TargetLocation = StartLocation + (CameraComp->GetForwardVector() * 10000.0f);

	FHitResult HitResult;
	bool bIsBlockingLine = GetWorld()->LineTraceSingleByObjectType(HitResult, StartLocation, TargetLocation, ObjectQueryParams);

	FRotator Rotation;
	if (bIsBlockingLine)
	{
		Rotation = UKismetMathLibrary::FindLookAtRotation(HandLocation, HitResult.Location);
	}
	else
	{
		Rotation = UKismetMathLibrary::FindLookAtRotation(HandLocation, TargetLocation);
	}

	// Tranform Spawn
	FTransform SpawnTM = FTransform(Rotation, HandLocation, FVector(1.0f));
	//FTransform SpawnTM = UKismetMathLibrary::MakeTransform(HandLocation, Rotation, FVector(1.0f));

	// Spawn Params
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	SpawnParams.Instigator = this;

	if (Object)
	{
		GetWorld()->SpawnActor<AActor>(Object, SpawnTM, SpawnParams);
	}

	// Clear All Timers
	GetWorldTimerManager().ClearTimer(PrimaryAttackTimerHandle);
	GetWorldTimerManager().ClearTimer(TeleportTimerHandle);
}

void AARogCharacter::OnHealthChange(AActor* InstigatorActor, UARogAttributeComponent* OwningComp, float NewHealth, float Delta)
{
	if (NewHealth <= 0 && Delta < 0.0f)
	{
		APlayerController* PC = Cast<APlayerController>(GetController());
		DisableInput(PC);
	}
}

//GEngine->AddOnScreenDebugMessage(-1, 4.5f, FColor::Purple, "Event Fired");


