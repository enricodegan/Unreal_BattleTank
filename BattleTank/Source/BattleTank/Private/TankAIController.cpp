// Copyright Enrico Degan 2019

#include "Tank.h"
#include "TankAIController.h"
#include "GameFramework/PlayerController.h"
#include "Engine/World.h"

// Depends on movement component via pathfinding system

void ATankAIController::BeginPlay()
{
	Super::BeginPlay();
}

void ATankAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	auto PlayerTank = Cast<ATank>(GetWorld()->GetFirstPlayerController()->GetPawn());
	auto ControlledTank = Cast<ATank>(GetPawn());

	if (ensure(PlayerTank)) 
	{ 
		// Move towards the player
		MoveToActor(PlayerTank, AcceptanceRadius);
		
		// Aim towards the player
		ControlledTank->AimAt(PlayerTank->GetActorLocation());

		// Fire if ready
		ControlledTank->Fire(); // TODO why is AI tank not firing?
	}
}