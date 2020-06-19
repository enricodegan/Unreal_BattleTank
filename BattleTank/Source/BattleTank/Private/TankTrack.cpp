// Fill out your copyright notice in the Description page of Project Settings.


#include "TankTrack.h"

// TODO fix keyboard implementation
void UTankTrack::SetThrottle(float Throttle)
{
	// TODO clamp throttle value so player can't override ***

	auto ForceApplied = GetForwardVector() * Throttle * TrackMaxDrivingForce;
	auto ForceLocation = GetComponentLocation();

	auto TankRoot = Cast<UPrimitiveComponent>(GetOwner()->GetRootComponent());
	TankRoot->AddForceAtLocation(ForceApplied, ForceLocation);
}
