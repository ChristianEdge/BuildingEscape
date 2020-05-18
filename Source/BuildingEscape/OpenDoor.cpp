// Copyright Christian Edge, 2020 


#include "OpenDoor.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"
#include "GameFramework/PlayerController.h"


UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}

void UOpenDoor::BeginPlay()	// Called when the game starts
{
	Super::BeginPlay();

	if (!PressurePlate)
	{
		UE_LOG(LogTemp, Error, TEXT("%s has no PressurePlate set!"), *GetOwner()->GetName());
	}

	if (!ActorThatEnters)
	{
		ActorThatEnters = GetWorld()->GetFirstPlayerController()->GetPawn();
	}

	InitialYaw = GetOwner()->GetActorRotation().Yaw;
	CurrentYaw = InitialYaw;
	DoorAngle += CurrentYaw;
}

void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{	// Called every frame
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (PressurePlate && PressurePlate->IsOverlappingActor(ActorThatEnters))
	{
		OpenDoor(DeltaTime);
		//UE_LOG(LogTemp, Warning, TEXT("Door opening"));
		CloseTime = GetWorld()->GetTimeSeconds() + CloseDelay;
	}
	else 
	{
		if (GetWorld()->GetTimeSeconds() > CloseTime)
		{
			CloseDoor(DeltaTime);
			//UE_LOG(LogTemp, Warning, TEXT("Door closing"));
		}
	}
}

void UOpenDoor::OpenDoor(float DeltaTime)
{
	//Smooth open, sudden stop. 
	//CurrentYaw = FMath::FInterpConstantTo(CurrentYaw, DoorAngle, DeltaTime, 45);

	//Seems slightly jerkier to me than FInterpConstantTo()
	//CurrentYaw = FMath::FInterpTo(CurrentYaw, DoorAngle, DeltaTime, 2);

	//Also smooth
	CurrentYaw = FMath::Lerp(CurrentYaw, DoorAngle, DeltaTime * OpenSpeed);

	DoorRotation = GetOwner()->GetActorRotation();
	DoorRotation.Yaw = CurrentYaw;
	
	//UE_LOG(LogTemp, Warning, TEXT("Rotation = %s"), *GetOwner()->GetActorRotation().ToString());
	//UE_LOG(LogTemp, Warning, TEXT("Yaw = %f"), GetOwner()->GetActorRotation().Yaw);
	
	GetOwner()->SetActorRotation(DoorRotation);
}

void UOpenDoor::CloseDoor(float DeltaTime)
{
	CurrentYaw = FMath::Lerp(CurrentYaw, InitialYaw, DeltaTime * CloseSpeed);
 
	DoorRotation = GetOwner()->GetActorRotation();
	DoorRotation.Yaw = CurrentYaw;

	GetOwner()->SetActorRotation(DoorRotation);
}