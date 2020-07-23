// Copyright Enrico Degan 2019

#include "Tank.h" // Implemented for cast on death
#include "TankPlayerController.h"
#include "TankAimingComponent.h"
#include "BattleTank.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "Components/SceneComponent.h"


void ATankPlayerController::BeginPlay()
{
	Super::BeginPlay();

	auto ControlledTank = GetPawn();
	if (ensure(ControlledTank))
	{
		UE_LOG(LogTemp, Warning, TEXT("Player Controller not possessing a Tank"))
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Player Controller IS possessing: %s"), *(ControlledTank->GetName()));
	}

	if (!GetPawn()) return;
	auto AimingComponent = GetPawn()->FindComponentByClass<UTankAimingComponent>();
	if (ensure(AimingComponent))
	{
		FoundAimingComponent(AimingComponent);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Player Controller can't find aiming component at Begin Play"))
	}
}

//Tick
void ATankPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	AimTowardsCrosshair();
}

void ATankPlayerController::AimTowardsCrosshair()
{
	if (!GetPawn()) { return; } // Not using `ensure` because the player might not always be possessing a tank (i.e. when they're dead)

	auto AimingComponent = GetPawn()->FindComponentByClass<UTankAimingComponent>();
	if (ensure(AimingComponent))
	{
		FoundAimingComponent(AimingComponent);
	}

	FVector HitLocation; // OUT parameter
	if (GetSightRayHitLocation(HitLocation))
	{
		AimingComponent->AimAt(HitLocation);
	}
}

bool ATankPlayerController::GetSightRayHitLocation(FVector& OutHitLocation) const
{
	/// Find the cross hair position in pixel coordinates
	int32 ViewportSizeX, ViewportSizeY;
	GetViewportSize(ViewportSizeX, ViewportSizeY);
	auto ScreenLocation = FVector2D(CrosshairXLocation * ViewportSizeX, CrosshairYLocation * ViewportSizeY);

	// Deproject the screen position of the cross hair to a world direction
	FVector LookDirection;
	if (GetLookDirection(ScreenLocation, LookDirection))
	{
		// Line trace along LookDirection, and see what we hit (up to a maximum range)
		return GetLookVectorHitLocation(ScreenLocation, OutHitLocation);
	}
	return false;
}

bool ATankPlayerController::GetLookDirection(FVector2D ScreenLocation, FVector& LookDirection) const
{
	FVector CameraWorldLocation; // To be discarded
	return  DeprojectScreenPositionToWorld(
		ScreenLocation.X,
		ScreenLocation.Y,
		CameraWorldLocation,
		LookDirection
	);
}

bool ATankPlayerController::GetLookVectorHitLocation(FVector2D ScreenLocation, FVector& OutHitLocation) const
{
	FHitResult HitResult;
	if (GetHitResultAtScreenPosition(ScreenLocation, ECollisionChannel::ECC_Camera, false, HitResult))
	{
		OutHitLocation = HitResult.Location; // Set hit location
		return true;
	}
	OutHitLocation = FVector(0);
	return false; // Line trace didn't succeed

	/* How Unreal Course Accomplished the LineTrace:
	|| Check "Lecture 210. Creating an Out Parameter Method" to "Lecture 213. Using LineTraceSingleByChannel()" for more information...
	||
	||	FHitResult HitResult;
	||	auto StartLocation = PlayerCameraManager->GetCameraLocation();
	||	auto EndLocation = StartLocation + (LookDirection * LineTraceRange);
	||
	||
	||	if (GetWorld()->LineTraceSingleByChannel(
	||			HitResult,
	||			StartLocation,
	||			EndLocation,
	||			ECollisionChannel::ECC_Visibility)
	||		)
	||	{
	||		OutHitLocation = HitResult.Location; // Set hit location
	||		return true;
	||	}
	||	OutHitLocation = FVector(0);
	||	return false; // Line trace didn't succeed
	||
	*/
}

void ATankPlayerController::SetPawn(APawn* InPawn)
{
	Super::SetPawn(InPawn);

	if (InPawn)
	{
		auto PossessedTank = Cast<ATank>(InPawn);
		if (!ensure(PossessedTank)) { return; }

		// Subscribe our local method to the tank's death event
		PossessedTank->OnDeath.AddUniqueDynamic(this, &ATankPlayerController::OnPlayerTankDeath);
	}
}

void ATankPlayerController::OnPlayerTankDeath()
{
	StartSpectatingOnly();
}