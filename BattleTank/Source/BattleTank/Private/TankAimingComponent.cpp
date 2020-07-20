// Copyright Enrico Degan 2019

#include "TankBarrel.h"
#include "TankTurret.h"
#include "Projectile.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Actor.h"
#include "Components/SceneComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "TankAimingComponent.h"

// Sets default values for this component's properties
UTankAimingComponent::UTankAimingComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// Set the Projectile BP via C++ (due to bug) (unfortunately it has to be hard-coded)
		// TODO is setting Projectile BP through C++ working?
	static ConstructorHelpers::FClassFinder<AProjectile> Proj(TEXT("/Game/Projectile/Projectile_BP"));
	if (Proj.Class)
	{
		ProjectileBlueprint = Proj.Class;
	}
}

void UTankAimingComponent::BeginPlay()
{
	Super::BeginPlay();

	// First fire is after initial reload
	LastFireTime = FPlatformTime::Seconds();
}

void UTankAimingComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (GetRoundsLeft() <= 0)
	{
		FiringState = EFiringState::OutOfAmmo;
	}
	else if ((FPlatformTime::Seconds() - LastFireTime) < ReloadTimeInSeconds)
	{
		FiringState = EFiringState::Reloading;
	}
	else if (IsBarrelMoving())
	{
		FiringState = EFiringState::Aiming;
	}
	else
	{
		FiringState = EFiringState::Locked;
	}
}

bool UTankAimingComponent::IsBarrelMoving()
{
	if (!ensure(Barrel)) { return false; }
	
	auto BarrelForwardVector = Barrel->GetForwardVector();
	auto Tolerance = 5;
	return !BarrelForwardVector.Equals(TargetAimDirection, 0.01);
}

int32 UTankAimingComponent::GetRoundsLeft() const
{
	return RoundsLeft;
}

EFiringState UTankAimingComponent::GetFiringState() const
{
	return FiringState;
}

void UTankAimingComponent::SetBarrelReference(UTankBarrel* BarrelToSet)
{
	if (!ensure(BarrelToSet)) { return; }
	Barrel = BarrelToSet; // This method is going to be called, and the barrel set, in Blueprints.
}

void UTankAimingComponent::SetTurretReference(UTankTurret* TurretToSet)
{
	if (!ensure(TurretToSet)) { return; }
	Turret = TurretToSet; // This method is going to be called, and the barrel set, in Blueprints.
}

void UTankAimingComponent::AimAt(FVector HitLocation)
{
	if (!ensure(Barrel) || !ensure(Turret)) { return; }

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
		TargetAimDirection = OutLaunchVelocity.GetSafeNormal();
		MoveBarrelTowards(TargetAimDirection);
	}
	//else
	//{
	//	auto Time = GetWorld()->GetTimeSeconds();
	//	UE_LOG(LogTemp, Warning, TEXT("%f: NO aim solution found"), Time)
	//}
}

// TODO give player full control over the turret (no influence from bHasAimSolution)
	// A separate Turret Movement function allows the player to have full control over the turret (while the Barrel adjusts automatically)
void UTankAimingComponent::MoveBarrelTowards(FVector TargetAimDirection)
{
	if (!ensure(Barrel) || !ensure(Turret)) { return; }
	
	// Get current barrel position
	auto BarrelRotator = Barrel->GetForwardVector().Rotation();
	// Get the difference between desired barrel position (TargetAimDirection) and current barrel position
	auto AimAsRotator = TargetAimDirection.Rotation();
	auto DeltaRotator = AimAsRotator - BarrelRotator;

	// Always rotate along the shortest path
	// TODO figure out why FMath::Abs is giving errors
	//if (FMath::Abs(DeltaRotator.Yaw()) < 180)
	//{
		Turret->Rotate(DeltaRotator.Yaw);
	//}
	//else
	//{
	//	Turret->Rotate(-(DeltaRotator.Yaw));
	//}

	Barrel->Elevate(DeltaRotator.Pitch);
}

void UTankAimingComponent::Fire()
{
	if (FiringState == EFiringState::Aiming || FiringState == EFiringState::Locked)
	{
		if (!ensure(Barrel)) { return; }
		if (!ensure(ProjectileBlueprint)) { return; }
		
		// Spawn a projectile at the socket location on the barrel
		auto Projectile = GetWorld()->SpawnActor<AProjectile>(
			ProjectileBlueprint,
			Barrel->GetSocketLocation(FName("Projectile")),
			Barrel->GetSocketRotation(FName("Projectile"))
			);

		Projectile->LaunchProjectile(LaunchSpeed);
		LastFireTime = FPlatformTime::Seconds();
		RoundsLeft--;
	}
}