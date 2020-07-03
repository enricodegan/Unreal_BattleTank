// Copyright Enrico Degan 2019

#pragma once

#include "GameFramework/Pawn.h"
#include "Tank.generated.h" // ^^^ New classes ALWAYS go above! ^^^

UCLASS()
class BATTLETANK_API ATank : public APawn
{
	GENERATED_BODY()

	// Sets default values for this pawn's properties
	ATank();

//public:
	/// We retype the same method from the `TankAimingComponent` to make it accessible to the Tank_BP.
	//UFUNCTION(BlueprintCallable, Category = Setup)
	//void SetBarrelReference(UTankBarrel* BarrelToSet); 

	//UFUNCTION(BlueprintCallable, Category = Setup)
	//void SetTurretReference(UTankTurret* TurretToSet);
};
