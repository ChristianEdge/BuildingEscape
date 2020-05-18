// Copyright Christian Edge, 2020 

#include "SwingingDoor_InterpConstTo.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"
#include "GameFramework/PlayerController.h"

// Sets default values for this component's properties
USwingingDoor_InterpConstTo::USwingingDoor_InterpConstTo()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void USwingingDoor_InterpConstTo::BeginPlay()
{
	Super::BeginPlay();

	if (!PressurePlate)
	{
		UE_LOG(LogTemp, Error, TEXT("%s has no PressurePlate set!"), *GetOwner()->GetName());
	}

	if (!ActorThatEnters)
	{
		// Look through world, get the first player controller (if there is one), 
		// get the pawn its attached to. Assign the pawn to our actor
		ActorThatEnters = GetWorld()->GetFirstPlayerController()->GetPawn();
	}

	InitialYaw = GetOwner()->GetActorRotation().Yaw;
	CurrentYaw = InitialYaw;
	TargetYaw += CurrentYaw;
}


// Called every frame
void USwingingDoor_InterpConstTo::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (PressurePlate)
	{
		if (PressurePlate->IsOverlappingActor(ActorThatEnters))
		{
			OpenDoor(DeltaTime);
			CloseTime = GetWorld()->GetTimeSeconds() + CloseDelay;
		}
	}
	else 
	{
		if (GetWorld()->GetTimeSeconds() > CloseTime)
		{
			CloseDoor(DeltaTime);
		}
	}
}

void USwingingDoor_InterpConstTo::OpenDoor(float DeltaTime)
{ 
	CurrentYaw = FMath::FInterpConstantTo(CurrentYaw, TargetYaw, DeltaTime, OpenSpeed);

	DoorRotation = GetOwner()->GetActorRotation();
	DoorRotation.Yaw = CurrentYaw;
	
	GetOwner()->SetActorRotation(DoorRotation);
}

void USwingingDoor_InterpConstTo::CloseDoor(float DeltaTime)
{
	CurrentYaw = FMath::FInterpConstantTo(CurrentYaw, TargetYaw, DeltaTime, CloseSpeed);
 
	DoorRotation = GetOwner()->GetActorRotation();
	DoorRotation.Yaw = CurrentYaw;

	GetOwner()->SetActorRotation(DoorRotation);
}
