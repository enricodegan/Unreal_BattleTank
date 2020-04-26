// Fill out your copyright notice in the Description page of Project Settings.

#include "TankPlayerController.h"
#include "BattleTank.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "Components/SceneComponent.h"


void ATankPlayerController::BeginPlay()
{
	Super::BeginPlay();

	auto ControlledTank = GetControlledTank();

	if (!ControlledTank)
	{
		UE_LOG(LogTemp, Warning, TEXT("Player Controller not possessing a Tank"))
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Player Controller IS possessing: %s"), *(ControlledTank->GetName()));
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
	if (!GetControlledTank()) { return; }

	FVector HitLocation; // OUT parameter
	if (GetSightRayHitLocation(HitLocation))
	{
		UE_LOG(LogTemp, Warning, TEXT("Hitlocation: %s"), *HitLocation.ToString());
		// TODO : tell player-controlled tank to aim at this point
	}
}


bool ATankPlayerController::GetSightRayHitLocation(FVector& OutHitLocation) const
{
	/// Find the crosshair position in pixel coordinates
	int32 ViewportSizeX, ViewportSizeY;
	GetViewportSize(ViewportSizeX, ViewportSizeY);
	auto ScreenLocation = FVector2D(CrosshairXLocation * ViewportSizeX, CrosshairYLocation * ViewportSizeY);

	// Deproject the screen position of the crosshair to a world direction
	FVector LookDirection;
	if (GetLookDirection(ScreenLocation, LookDirection))
	{
		GetLookVectorHitLocation(LookDirection, OutHitLocation); // Line trace and see what we hit (up to max range)
	}
	return true;
}

bool ATankPlayerController::GetLookDirection(FVector2D ScreenLocation, FVector &OutLookDirection) const
{
	FVector CameraWorldLocation;
	return DeprojectScreenPositionToWorld(
		ScreenLocation.X, 
		ScreenLocation.Y, 
		CameraWorldLocation, 
		OutLookDirection // Unit vector
	);
}

bool ATankPlayerController::GetLookVectorHitLocation(FVector LookDirection, FVector& OutHitLocation) const
{
	// TODO : Replace with this function:
	// GetHitResultAtScreenPosition(ScreenLocation, ECC_Visibility, false, OutHitLocation);
	// https://www.udemy.com/course/unrealcourse/learn/lecture/5231428#questions/10006844
	
	FHitResult HitResult;
	auto StartLocation = PlayerCameraManager->GetCameraLocation();
	auto EndLocation = StartLocation + (LookDirection * LineTraceRange);
	if (GetWorld()->LineTraceSingleByChannel(
			HitResult, 
			StartLocation, 
			EndLocation, 
			ECollisionChannel::ECC_Visibility)
		)
	{
		OutHitLocation = HitResult.Location; // Set hit location
		return true;
	}
	OutHitLocation = FVector(0);
	return false; // Line trace didn't succeed
}
