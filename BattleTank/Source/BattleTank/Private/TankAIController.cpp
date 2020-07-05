// Copyright Enrico Degan 2019

#include "TankAimingComponent.h"
#include "TankAIController.h"
#include "GameFramework/PlayerController.h"
#include "Engine/World.h"
// Depends on movement component via path finding system

void ATankAIController::BeginPlay()
{
	Super::BeginPlay();
}

void ATankAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// You don't actually have to cast since the Tank already "is-a" pawn; therefore, it already inherits a pawn class -- no need to cast!
	auto PlayerTank = GetWorld()->GetFirstPlayerController()->GetPawn();
	auto ControlledTank = GetPawn();

	if (!ensure(PlayerTank && ControlledTank)) { return; }
	
	// Move towards the player
	MoveToActor(PlayerTank, AcceptanceRadius);
		
	// Aim towards the player
	auto AimingComponent = GetPawn()->FindComponentByClass<UTankAimingComponent>();
	AimingComponent->AimAt(PlayerTank->GetActorLocation());

	// Fire when locked
	if (AimingComponent->GetFiringState() == EFiringState::Locked)
	{
		AimingComponent->Fire(); // TODO why is AI tank not firing?
	}
}