// Copyright Enrico Degan 2019

#pragma once

#include "GameFramework/Pawn.h"
#include "Tank.generated.h" // ^^^ New classes ALWAYS go above! ^^^

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FTankDelegate);

UCLASS()
class BATTLETANK_API ATank : public APawn
{
	GENERATED_BODY()

public:
	FTankDelegate OnDeath();

private:
	// Sets default values for this pawn's properties
	ATank();

	virtual void BeginPlay() override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;

	/** 
	 * Derived from AActor (
	 *	 TODO ask why are we not deriving it from APawn since the tank "is-a" pawn?
	 * Apply damage to this actor.
	 * @see https://www.unrealengine.com/blog/damage-in-ue4
	 * @param DamageAmount		How much damage to apply
	 * @param DamageEvent		Data package that fully describes the damage received.
	 * @param EventInstigator	The Controller responsible for the damage.
	 * @param DamageCauser		The Actor that directly caused the damage (e.g. the projectile that exploded, the rock that landed on you)
	 * @return					The amount of damage actually applied.
	 */
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	// Returns Current Health as a percentage of StartingHealth (between 0 and 1)
		// BlueprintPure is basically like `const`, but for BP; it makes a read-only function that can't change any values
	UFUNCTION(BlueprintPure, Category = "Tank Health")
	float GetHealthPercent() const;

	UPROPERTY(EditDefaultsOnly, Category = "Tank Health")
	int32 StartingHealth = 100;

	UPROPERTY(VisibleAnywhere, Category = "Tank Health")
	int32 CurrentHealth; // Initialized in Beginplay

	/**
	 * PRE-REFACTORING (Lecture 186-187 Archived Course)
	 */
		// APawn
		//virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	//public:
		/// We retype the same method from the `TankAimingComponent` to make it accessible to the Tank_BP.
		//UFUNCTION(BlueprintCallable, Category = Setup)
		//void SetBarrelReference(UTankBarrel* BarrelToSet); 

		//UFUNCTION(BlueprintCallable, Category = Setup)
		//void SetTurretReference(UTankTurret* TurretToSet);
};
