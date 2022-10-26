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

	UPROPERTY(EditDefaultsOnly, Category = Ability)
	TSubclassOf<AActor> MagicProjectileClass;

	UPROPERTY(EditDefaultsOnly, Category = Ability)
	TSubclassOf<AActor> TeleportProjectileClass;

	UPROPERTY(EditDefaultsOnly, Category = Ability)
	TSubclassOf<AActor> BlackHoleProjectileClass;

	UPROPERTY(EditDefaultsOnly, Category = Ability)
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

	// Called between Constructor and Begin Play (Called after Contructor and before Begin Play)
	virtual void PostInitializeComponents() override;

	// Movement
	void MoveForward(float Value);
	void MoveRight(float Value);

	// Start Play Attack Effects
	void StartAttackEffects();

	UPROPERTY(EditAnywhere, Category = Ability)
	UParticleSystem* CastingEffect;
	
	FName HandSocketName;

	// Actions
	void PrimaryAttack();
	FTimerHandle PrimaryAttackTimerHandle;
	
	// Handle Spawn Class After a period of time
	void AbilityTimerElapsed(TSubclassOf<AActor> Object);

	// Interaction with world using Interface
	void PrimaryInteract();

	// Teleport the actor using a projectile
	void TeleportAbility();
	FTimerHandle TeleportTimerHandle;
	
	// Secundary Attack
	void SecundaryAttack();
	FTimerHandle SecundaryAttackTimerHandle;

	UFUNCTION()
	void OnHealthChange(AActor* InstigatorActor, UARogAttributeComponent* OwningComp, float NewHealth, float Delta);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	UFUNCTION(Exec)
	void HealtSelf(float Amount = 100.f);

};

// The best way to arganize the .h file is separete variables and functions by type.
