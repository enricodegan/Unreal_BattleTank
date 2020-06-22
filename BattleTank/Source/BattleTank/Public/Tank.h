// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Pawn.h"
#include "Tank.generated.h" // ^^^ New classes ALWAYS go above! ^^^

/// Forward Declaration
class UTankBarrel;
//class UTankTurret; // TODO delete? Doesn't seem like it's needed...
class AProjectile;
class UTankAimingComponent; 
class UTankMovementComponent;

UCLASS()
class BATTLETANK_API ATank : public APawn
{
	GENERATED_BODY()

public:
	/// We retype the same method from the `TankAimingComponent` to make it accessible to the Tank_BP.
	UFUNCTION(BlueprintCallable, Category = Setup)
	void SetBarrelReference(UTankBarrel* BarrelToSet); 

	UFUNCTION(BlueprintCallable, Category = Setup)
	void SetTurretReference(UTankTurret* TurretToSet);

	void AimAt(FVector HitLocation);

	UFUNCTION(BlueprintCallable, Category = Firing)
	void Fire();

protected:
	UPROPERTY(BlueprintReadOnly)
	UTankAimingComponent* TankAimingComponent = nullptr;

	UPROPERTY(BlueprintReadOnly)
	UTankMovementComponent* TankMovementComponent = nullptr;

private:
	// Sets default values for this pawn's properties
	ATank();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;

	UPROPERTY(EditDefaultsOnly, Category = Setup)
	TSubclassOf<AProjectile> ProjectileBlueprint;

	UPROPERTY(EditDefaultsOnly, Category = Firing)
	float LaunchSpeed = 4000;

	UPROPERTY(EditDefaultsOnly, Category = Firing)
	float ReloadTimeInSeconds = 3;

	double LastFireTime = 0;

	// Local barrel reference for spawning projectile
	UTankBarrel* Barrel = nullptr;
};
