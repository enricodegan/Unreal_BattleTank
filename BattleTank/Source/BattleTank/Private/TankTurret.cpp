// Fill out your copyright notice in the Description page of Project Settings.


#include "TankTurret.h"

void UTankTurret::Rotate(float RelativeSpeed)
{
	// Move the turret the difference between current and desired
		// Given the max rotation speed and the frame time
	RelativeSpeed = FMath::Clamp<float>(RelativeSpeed, -1, +1);
	auto RotationChange = RelativeSpeed * MaxDegreesPerSecond * GetWorld()->DeltaTimeSeconds;
	
	// RelativeRotation is a function of the FRotator USceneComponenet::RelativeRotation class. 
	//It gives the rotation of a component relative to its parent.
		// `.Yaw` gives us the rotation.
	auto RawNewRotation = RelativeRotation.Yaw + RotationChange;
	
	// TODO fix turret rotation bugs
		// tank can aim at itself
		// turning gets messed up?
	SetRelativeRotation(FRotator(0, RawNewRotation, 0));
	

}
