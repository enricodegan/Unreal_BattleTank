// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Tank.h"
#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "TankPlayerController.generated.h" // MUST be the last include


UCLASS()
class BATTLETANK_API ATankPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ATank* GetControlledTank() const;
	
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CrosshairXLocation = 0.5;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CrosshairYLocation = 0.33333;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float LineTraceRange = 1000000.0;

private:
	// Start the tank moving the barrel so that a shot would it where
	// the crosshair intersects the world
	void AimTowardsCrosshair();

	bool GetSightRayHitLocation(FVector&) const;
	bool GetLookDirection(FVector2D ScreenLocation, FVector&) const;
	bool GetLookVectorHitLocation(FVector LookDirection, FVector& OutHitLocation) const;

	FVector2D ScreenLocation;
	FVector CameraWorldLocation;

};
