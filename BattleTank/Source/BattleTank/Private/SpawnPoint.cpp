// Copyright Enrico Degan 2019


#include "SpawnPoint.h"
#include <Kismet/GameplayStatics.h>
#include <ConstructorHelpers.h>

// Sets default values for this component's properties
USpawnPoint::USpawnPoint()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// Automatically set the SpringWheel_BP as the SpawnClass object
	static ConstructorHelpers::FClassFinder<ASpringWheel> Springs(TEXT("/Game/Tank/SpringWheel_BP"));
	if (Springs.Class)
	{
		SpawnClass = Springs.Class;
	}
}

// Called when the game starts
void USpawnPoint::BeginPlay()
{
	Super::BeginPlay();

	SpawnedActor = GetWorld()->SpawnActorDeferred<AActor>(SpawnClass, GetComponentTransform());
	if (!SpawnedActor) { return; }
	SpawnedActor->AttachToComponent(this, FAttachmentTransformRules::KeepWorldTransform);
	UGameplayStatics::FinishSpawningActor(SpawnedActor, GetComponentTransform());
}

// Called every frame
void USpawnPoint::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}