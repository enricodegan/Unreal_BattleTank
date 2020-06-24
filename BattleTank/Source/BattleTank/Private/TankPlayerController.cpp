// Copyright Enrico Degan 2019

#include "Tank.h"
#include "TankPlayerController.h"
#include "TankAimingComponent.h"
#include "BattleTank.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "Components/SceneComponent.h"


void ATankPlayerController::BeginPlay()
{
	Super::BeginPlay();

	auto ControlledTank = GetControlledTank();
	if (ensure(ControlledTank))
	{
		UE_LOG(LogTemp, Warning, TEXT("Player Controller not possessing a Tank"))
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Player Controller IS possessing: %s"), *(ControlledTank->GetName()));
	}

	auto AimingComponent = GetControlledTank()->FindComponentByClass<UTankAimingComponent>();
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

ATank* ATankPlayerController::GetControlledTank() const
{
	return Cast<ATank>(GetPawn());
}

void ATankPlayerController::AimTowardsCrosshair()
{
	if (!ensure(GetControlledTank())) { return; }

	FVector HitLocation; // OUT parameter
	if (GetSightRayHitLocation(HitLocation))
	{
		GetControlledTank()->AimAt(HitLocation);
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
		GetLookVectorHitLocation(ScreenLocation, OutHitLocation);
	}
	return true;
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
	if (GetHitResultAtScreenPosition(ScreenLocation, ECollisionChannel::ECC_Visibility, false, HitResult))
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
