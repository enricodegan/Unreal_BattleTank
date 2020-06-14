// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

//#include "Tank.h"
#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "TankPlayerController.generated.h" // MUST be the last include

class ATank; // Forward Declaration

UCLASS()
class BATTLETANK_API ATankPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ATank* GetControlledTank() const;
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaSeconds) override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float CrosshairXLocation = 0.5;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float CrosshairYLocation = 0.33333;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float LineTraceRange = 1000000.0;

private:
	// Start the tank moving the barrel so that a shot would it where
	// the crosshair intersects the world
	void AimTowardsCrosshair();

	bool GetSightRayHitLocation(FVector&) const;
	bool GetLookVectorHitLocation(FVector2D ScreenLocation, FVector& OutHitLocation) const;

	FVector2D ScreenLocation;

};
