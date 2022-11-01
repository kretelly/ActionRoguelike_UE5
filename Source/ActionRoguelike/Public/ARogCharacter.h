// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameplayTagContainer.h"
#include "ARogCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;
class UARogInteractionComponent;
class UARogAttributeComponent;
class UARogActionComponent;
class UAnimMontage;

UCLASS()
class ACTIONROGUELIKE_API AARogCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AARogCharacter();

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

	// Action component
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UARogActionComponent* ActionComp;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called between Constructor and Begin Play (Called after Contructor and before Begin Play)
	virtual void PostInitializeComponents() override;

	// Movement
	void MoveForward(float Value);
	void MoveRight(float Value);

	// Sprint Ability
	void SprintStart();
	void SprintStop();

	// Actions
	void PrimaryAttack();

	// Interaction with world using Interface
	void PrimaryInteract();

	// Teleport the actor using a projectile
	void TeleportAbility();
	
	// Secundary Attack
	void SecundaryAttack();

	// Parry Defense
	void ToggleParry();

	UFUNCTION()
	void OnHealthChange(AActor* InstigatorActor, UARogAttributeComponent* OwningComp, float NewHealth, float Delta);

	// We're overriding this function to fix the GetActorEyesViewPoint, simple way to fix the Interact Component.
	virtual FVector GetPawnViewLocation() const override;

	UPROPERTY(EditDefaultsOnly, Category = Attack)
	FGameplayTag ParryTag;

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	UFUNCTION(Exec)
	void HealtSelf(float Amount = 100.f);

};

// The best way to arganize the .h file is separete variables and functions by type.
