// Fill out your copyright notice in the Description page of Project Settings.

#include "TankBarrel.h"
#include "TankTurret.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Actor.h"
#include "Components/SceneComponent.h"
#include "TankAimingComponent.h"

// Sets default values for this component's properties
UTankAimingComponent::UTankAimingComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}

void UTankAimingComponent::SetBarrelReference(UTankBarrel* BarrelToSet)
{
	if (!BarrelToSet) { return; }
	Barrel = BarrelToSet; // This method is going to be called, and the barrel set, in Blueprints.
}

void UTankAimingComponent::SetTurretReference(UTankTurret* TurretToSet)
{
	if (!TurretToSet) { return; }
	Turret = TurretToSet; // This method is going to be called, and the barrel set, in Blueprints.
}

void UTankAimingComponent::AimAt(FVector HitLocation, float LaunchSpeed)
{
	if (!Barrel) { return; }
	if (!Turret) { return; }

	/*
	// Turret Movement
	auto AimRotation = HitLocation.GetSafeNormal();
	MoveTurretTowards(AimRotation);
	*/

	// Barrel Movement
	FVector OutLaunchVelocity;
	FVector StartLocation = Barrel->GetSocketLocation(FName("Projectile"));
	bool bHasAimSolution = UGameplayStatics::SuggestProjectileVelocity( // Calculate the Launch Velocity
		this,
		OutLaunchVelocity,
		StartLocation,
		HitLocation,
		LaunchSpeed,
		// These parameters below have default parameters, so we shouldn't need to put these values in.
		// HOWEVER, because of a bug in UE4 these still have to be written in.
		false,
		0.0f,
		0.0f,
		ESuggestProjVelocityTraceOption::DoNotTrace
	);
	if (bHasAimSolution)
	{
		// GetSafeNormal() gets the normalized vector (a unit vector)
		auto AimDirection = OutLaunchVelocity.GetSafeNormal();
		MoveBarrelTowards(AimDirection);
	}
	else
	{
		auto Time = GetWorld()->GetTimeSeconds();
		UE_LOG(LogTemp, Warning, TEXT("%f: NO aim solution found"), Time)
	}
}

void UTankAimingComponent::MoveBarrelTowards(FVector AimDirection)
{
	// Get current barrel position
	auto BarrelRotator = Barrel->GetForwardVector().Rotation();
	// Get the difference between desires barrel position (AimDirection) and current barrel position
	auto AimAsRotator = AimDirection.Rotation();
	auto DeltaRotator = AimAsRotator - BarrelRotator;

	Barrel->Elevate(DeltaRotator.Pitch);
	Turret->Rotate(DeltaRotator.Yaw);
}

/*
// TODO give player full control over the turret (no influence from bHasAimSolution)
	// A separate Turret Movement function allows the player to have full control over the turret (while the Barrel adjusts automatically)
void UTankAimingComponent::MoveTurretTowards(FVector AimRotation)
{
	// Get current turret position
	auto TurretRotator = Turret->GetForwardVector().Rotation();
	// Get the the difference between desires turret position (AimRotation) and current turret position
	auto RotationAsRotator = AimRotation.Rotation();
	auto DeltaRotator = RotationAsRotator - TurretRotator;

	Turret->Rotate(DeltaRotator.Yaw);
}
*/