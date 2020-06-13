// Fill out your copyright notice in the Description page of Project Settings.


#include "TankBarrel.h"

void UTankBarrel::Elevate(float RelativeSpeed)
{
	// Move the barrel the difference between current and desired
		// Given the max elevation speed and the frame time
	auto ElevationChange = FMath::Clamp<float>(RelativeSpeed, -1, +1) * MaxDegreesPerSecond * GetWorld()->DeltaTimeSeconds;
	
	// RelativeRotation is a function of the FRotator USceneComponenet::RelativeRotation class. 
	//It gives the rotation of a component relative to its parent.
		// `.Pitch` gives us the elevation.
	auto RawNewElevation = RelativeRotation.Pitch + ElevationChange;
	
	float ClampedElevation = FMath::Clamp(RawNewElevation, MinElevationInDegrees, MaxElevationInDegrees);
	// UE_LOG(LogTemp, Warning, TEXT("Current elevation (clamped): %f"), ClampedElevation)

	SetRelativeRotation(FRotator(ClampedElevation, 0, 0));
	
}