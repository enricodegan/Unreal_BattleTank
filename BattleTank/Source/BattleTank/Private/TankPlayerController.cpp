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


	GetLookVectorHitLocation(ScreenLocation, OutHitLocation);
	return true;
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
