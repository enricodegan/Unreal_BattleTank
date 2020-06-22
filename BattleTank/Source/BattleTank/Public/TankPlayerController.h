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

protected:
	UFUNCTION(BlueprintCallable, Category = Setup)
	ATank* GetControlledTank() const;

private:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaSeconds) override;

	// Start the tank moving the barrel so that a shot would it be where
	// the cross hair intersects the world
	void AimTowardsCrosshair();

	// Returns an OUT parameter, true if it hits landscape (because it's a bool)
	bool GetSightRayHitLocation(FVector&) const;

	UPROPERTY(EditDefaultsOnly)
	float CrosshairXLocation = 0.5;

	UPROPERTY(EditDefaultsOnly)
	float CrosshairYLocation = 0.33333;

	UPROPERTY(EditDefaultsOnly)
	float LineTraceRange = 1000000.0;

	bool GetLookDirection(FVector2D ScreenLocation, FVector& LookDirection) const;
	bool GetLookVectorHitLocation(FVector2D ScreenLocation, FVector& OutHitLocation) const;
};
