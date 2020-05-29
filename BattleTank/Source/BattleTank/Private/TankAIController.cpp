// Fill out your copyright notice in the Description page of Project Settings.

#include "TankAIController.h"
#include "GameFramework/PlayerController.h"
#include "Engine/World.h"


void ATankAIController::BeginPlay()
{
	Super::BeginPlay();

	auto AITank = GetControlledTank();

	if (!AITank)
	{
		UE_LOG(LogTemp, Warning, TEXT("AI not possessing a Tank"))
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("AI Controller IS possessing: %s"), *(AITank->GetName()));
	}

	auto PlayerTank = GetPlayerTank();
	if (!PlayerTank)
	{
		UE_LOG(LogTemp, Warning, TEXT("AI not finding player Tank. Check defaults in Gamemode BP."))
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("AI found Tank: %s"), *(AITank->GetName()));
	}
}

void ATankAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (GetPlayerTank()) 
	{ 
		// TODO : Move towards the player
		
		// Aim towards the player
		GetControlledTank()->AimAt(GetPlayerTank()->GetActorLocation());

		// Fire if ready
		
	}
}

ATank* ATankAIController::GetControlledTank() const
{
	return Cast<ATank>(GetPawn());
}

ATank* ATankAIController::GetPlayerTank() const
{
	auto PlayerPawn = Cast<ATank>(GetWorld()->GetFirstPlayerController()->GetPawn());
	// ^^^ This pointer has no reason to protected because the Casting will fail if the pointer is either nullptr or an invalid ptr.
	return Cast<ATank>(PlayerPawn);

}