// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ARogCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;
class UARogInteractionComponent;
class UARogAttributeComponent;
class UAnimMontage;

UCLASS()
class ACTIONROGUELIKE_API AARogCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AARogCharacter();

protected:

	UPROPERTY(EditAnywhere, Category = Ability)
	TSubclassOf<AActor> MagicProjectileClass;

	UPROPERTY(EditAnywhere, Category = Ability)
	TSubclassOf<AActor> TeleportProjectileClass;

	UPROPERTY(EditAnywhere, Category = Ability)
	UAnimMontage* AttackAnim;

protected:
	
	// Camera boom -> it posits the camera behind the character 
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	USpringArmComponent* SpringArmComp;
	
	// Follow camera
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	UCameraComponent* CameraComp;

	// Interaction using interface
	UPROPERTY(VisibleAnywhere, Category = "Components")
	UARogInteractionComponent* InteractionComp;

	// Attributes component
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UARogAttributeComponent* AttributeComp;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Movement
	void MoveForward(float Value);
	void MoveRight(float Value);

	// Actions
	void PrimaryAttack();
	FTimerHandle PrimaryAttackTimerHandle;
	
	// Handle Spawn Class After a period of time
	void PrimaryAbilityTimeElapsed(TSubclassOf<AActor> Object);

	// Interaction with world using Interface
	void PrimaryInteract();

	// Teleport the actor using a projectile
	void PrimaryTeleport();
	FTimerHandle PrimaryTeleportTimerHandle;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
