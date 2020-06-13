// Fill out your copyright notice in the Description page of Project Settings.


#include "TankTurret.h"

void UTankTurret::Rotate(float x)
{
	// Move the turret the difference between current and desired
		// Given the max rotation speed and the frame time
	auto RotationChange = x * MaxDegreesPerSecond * GetWorld()->DeltaTimeSeconds;
	
	
	
	
	
	
	// SetRelativeRotation(x /*cursor position*/);
	

}
