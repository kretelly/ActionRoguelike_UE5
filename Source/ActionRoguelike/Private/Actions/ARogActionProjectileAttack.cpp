// Fill out your copyright notice in the Description page of Project Settings.


#include "Actions/ARogActionProjectileAttack.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Camera/CameraComponent.h"
#include "ARogCharacter.h"

UARogActionProjectileAttack::UARogActionProjectileAttack()
{
	AttackAnimDelay = 0.2f;
	HandSocketName = "Muzzle_01";
}

void UARogActionProjectileAttack::StartAction_Implementation(AActor* Instigator)
{
	Super::StartAction_Implementation(Instigator);

	ACharacter* Character = Cast<ACharacter>(Instigator);
	if (Character)
	{
		Character->PlayAnimMontage(AttackAnim);
		UGameplayStatics::SpawnEmitterAttached(CastingEffect, Character->GetMesh(), HandSocketName, FVector::ZeroVector, FRotator::ZeroRotator, EAttachLocation::SnapToTarget);

		FTimerHandle AttackTimerHandle;
		FTimerDelegate TimerDelegate;
		TimerDelegate.BindUFunction(this, "AttackTimerElapsed", Character);
		GetWorld()->GetTimerManager().SetTimer(AttackTimerHandle, TimerDelegate, AttackAnimDelay, false);
	}
}

void UARogActionProjectileAttack::AttackTimerElapsed(ACharacter* InstigatorCharacter)
{
	if (ProjectileClass)
	{
		//Console Variable -> courses.tomlooman.com/courses/1320807/lectures/34466453/comments/17237945
		//static const auto CVar = IConsoleManager::Get().FindConsoleVariable(TEXT("ARog.InteractionDebugDraw"));
		static IConsoleVariable* ICVar = IConsoleManager::Get().FindConsoleVariable(TEXT("ARog.InteractionDebugDraw"));

		// Hand Location to Spawn Projectile
		FVector HandLocation = InstigatorCharacter->GetMesh()->GetSocketLocation(HandSocketName);

		// Target Location (using linetrace)
		FCollisionObjectQueryParams ObjectQueryParams;
		ObjectQueryParams.AddObjectTypesToQuery(ECollisionChannel::ECC_WorldStatic);
		ObjectQueryParams.AddObjectTypesToQuery(ECollisionChannel::ECC_WorldDynamic);
		ObjectQueryParams.AddObjectTypesToQuery(ECollisionChannel::ECC_PhysicsBody);
		ObjectQueryParams.AddObjectTypesToQuery(ECollisionChannel::ECC_Pawn);

		// Ignore Player
		FCollisionQueryParams QueryParams;
		QueryParams.AddIgnoredActor(InstigatorCharacter);

		/** Line Trace	
		// Get Camera Component
		UCameraComponent* CameraComp = InstigatorCharacter->FindComponentByClass<UCameraComponent>();

		FVector StartLocation = CameraComp->GetComponentLocation(); // InstigatorCharacter->GetPawnViewLocation() -> because we have overrided this function
		FVector TargetLocation = StartLocation + (CameraComp->GetForwardVector() * 10000.0f);

		FHitResult HitResult;
		bool bIsBlockingLine = GetWorld()->LineTraceSingleByObjectType(HitResult, StartLocation, TargetLocation, ObjectQueryParams, QueryParams);

		TargetLocation = bIsBlockingLine ? HitResult.Location : TargetLocation;
		FRotator Rotation = UKismetMathLibrary::FindLookAtRotation(HandLocation, TargetLocation);

		// CVar
		if (ICVar->GetBool()) DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, 32.0f, 32, FColor::Green, false, 2.0f, 0, 1.f);
		*/		

		// Sweep Trace
		FCollisionShape Shape;
		Shape.SetSphere(15.0f);

		// Get Camera Component
		UCameraComponent* CameraComp = InstigatorCharacter->FindComponentByClass<UCameraComponent>();

		FVector TraceStart = CameraComp->GetComponentLocation();
		FVector TraceEnd = TraceStart + (InstigatorCharacter->GetControlRotation().Vector() * 5000.0f);

		FHitResult HitResult;
		if (GetWorld()->SweepSingleByChannel(HitResult, TraceStart, TraceEnd, FQuat::Identity, ECC_GameTraceChannel1, Shape, QueryParams))
		{
			// Overwrite trace end with impact point in world
			TraceEnd = HitResult.ImpactPoint;
			// CVar
			if (ICVar->GetBool()) DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, 32.0f, 32, FColor::Green, false, 2.0f, 0, 1.f);
		}
		FRotator Rotation = FRotationMatrix::MakeFromX(TraceEnd - HandLocation).Rotator();

		// Tranform Spawn
		FTransform SpawnTM = FTransform(Rotation, HandLocation);

		// Spawn Params
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		SpawnParams.Instigator = InstigatorCharacter;

		if (ProjectileClass)
		{
			GetWorld()->SpawnActor<AActor>(ProjectileClass, SpawnTM, SpawnParams);
		}
	}
	StopAction(InstigatorCharacter);
}
