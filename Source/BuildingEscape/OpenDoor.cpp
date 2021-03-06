// Copyright Christian Edge, 2020 


#include "OpenDoor.h"
#include "Components/AudioComponent.h"
#include "Components/PrimitiveComponent.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"
#include "GameFramework/PlayerController.h"

#define OUT

UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}

void UOpenDoor::BeginPlay()	// Called when the game starts
{
	Super::BeginPlay();

	SetupPressurePlate();

	SetupAudioComponent();

	InitialYaw = GetOwner()->GetActorRotation().Yaw;
	CurrentYaw = InitialYaw;
	DoorAngle += CurrentYaw;
}

void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{	// Called every frame
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	// Trigger volume opnes door, time closes it
	if (TotalMassInTrigVol() > TriggerMass)
	{
		OpenDoor(DeltaTime); //UE_LOG(LogTemp, Warning, TEXT("Door opening"));
		CloseTime = GetWorld()->GetTimeSeconds() + CloseDelay;
	}
	else 
	{
		if (GetWorld()->GetTimeSeconds() > CloseTime)
		{
			CloseDoor(DeltaTime); //UE_LOG(LogTemp, Warning, TEXT("Door closing"));
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

	if (!AudioComponent) { return; }
	if (!AudioComponent->IsPlaying() && bIsClosed)
	{
		AudioComponent->Play();
		bIsClosed = false;
		bIsOpen = true;
	}
}

void UOpenDoor::CloseDoor(float DeltaTime)
{
	CurrentYaw = FMath::Lerp(CurrentYaw, InitialYaw, DeltaTime * CloseSpeed);
 
	DoorRotation = GetOwner()->GetActorRotation();
	DoorRotation.Yaw = CurrentYaw;

	GetOwner()->SetActorRotation(DoorRotation);

	if (!AudioComponent) { return; }
	if (!AudioComponent->IsPlaying() && bIsOpen)
	{
		AudioComponent->Play();
		bIsClosed = true;
		bIsOpen = false;
	}
}

// Calculates the total mass of objects within the trigger volume. 
float UOpenDoor::TotalMassInTrigVol() const
{
	float TotalMass = 0.0f;

	// Find all actors overlapping the Trigger Volume
	TArray<AActor*> OverlappingActors;
	if (!PressurePlate) { return TotalMass; }
	PressurePlate->GetOverlappingActors(OUT OverlappingActors);

	// Sum the mass of those actors
	for (AActor* Actor : OverlappingActors)
	{ 
		TotalMass += Actor->FindComponentByClass<UPrimitiveComponent>()->GetMass(); 
	}

	return TotalMass;
}

void UOpenDoor::SetupAudioComponent()
{
	AudioComponent = GetOwner()->FindComponentByClass<UAudioComponent>();

	if (!AudioComponent)
	{
		UE_LOG(LogTemp, Error, TEXT("AudioComponent missing from actor %s"), *GetOwner()->GetName());
	}
}

void UOpenDoor::SetupPressurePlate()
{
	if (!PressurePlate)
	{
		UE_LOG(LogTemp, Error, TEXT("%s has no PressurePlate set!"), *GetOwner()->GetName());
	}
}