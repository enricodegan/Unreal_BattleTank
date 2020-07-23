// Fill out your copyright notice in the Description page of Project Settings.

#include "SpringWheel.h"
#include "TankTrack.h"
#include "SpawnPoint.h"

UTankTrack::UTankTrack()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}

TArray<ASpringWheel*> UTankTrack::GetWheels() const
{
	TArray<ASpringWheel*> ResultArray;
	TArray<USceneComponent*> Children;
	GetChildrenComponents(true, Children);
	for (USceneComponent* Child : Children)
	{
		// The casts here are used to check if the object we are getting (from Child) is actually of type `USpawnPoint`.
		// If it isn't, the Child is cast and becomes a USpawnPoint.
		auto SpawnPointChild = Cast<USpawnPoint>(Child);
		if (!SpawnPointChild) continue;
		
		AActor* SpawnedChild = SpawnPointChild->GetSpawnedActor();
		auto SpringWheel = Cast<ASpringWheel>(SpawnedChild);
		if(!SpringWheel) continue;

		ResultArray.Add(SpringWheel);
	}
	return ResultArray;
}

// TODO fix keyboard implementation
void UTankTrack::SetThrottle(float Throttle)
{
	float CurrentThrottle = FMath::Clamp<float>(Throttle, -1, +1);
	DriveTrack(CurrentThrottle);
}

void UTankTrack::DriveTrack(float CurrentThrottle)
{
	// TODO clamp throttle value so player can't override ***

	auto ForceApplied = CurrentThrottle * TrackMaxDrivingForce;
	auto Wheel = GetWheels();
	auto ForcePerWheel = ForceApplied / Wheel.Num();
	for (ASpringWheel* Wheels : Wheel)
	{
		Wheels->AddDrivingForce(ForcePerWheel);
	}
}

// *** LEGACY CODE ***
	//void UTankTrack::BeginPlay()
	//{
	//	Super::BeginPlay();
	//
	//	OnComponentHit.AddDynamic(this, &UTankTrack::OnHit);
	//}
	//
	//void UTankTrack::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
	//{
	//	DriveTrack();
	//	ApplyCorrectingForce();
	//	CurrentThrottle = 0;
	//}
	//
	//void UTankTrack::ApplyCorrectingForce()
	//{
	//	// Calculate slippage (deviation) speed
	//	auto SlippageSpeed = FVector::DotProduct(GetRightVector(), GetComponentVelocity());
	//
	//	auto DeltaTime = GetWorld()->GetDeltaSeconds();
	//
	//	// Work out the required acceleration this frame to correct
	//		/* Acceleration is in m/s^2.
	//			* Therefore, Speed / DeltaTime gives the acceleration.
	//			* *GetRightVecotr() converts it to a vector
	//			* - applies the force in the opposite direction
	//			*/
	//	auto CorrectionAcceleration = -(SlippageSpeed / DeltaTime * GetRightVector());
	//
	//	// Calculate and apply sideways force (F = m * a)
	//	auto TankRoot = Cast<UStaticMeshComponent>(GetOwner()->GetRootComponent());
	//	auto CorrectionForce = (TankRoot->GetMass() * CorrectionAcceleration) / 2;	// Divide by 2 becuase a Tank has 2 tracks;
	//	TankRoot->AddForce(CorrectionForce);
	//}
 