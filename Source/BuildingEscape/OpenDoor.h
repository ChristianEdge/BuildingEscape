// Copyright Christian Edge, 2020
/*
 * OpenDoor gets attached to a movable object that will swing like a door when
 * a trigger volume has enough mass. Essentially a pressure plate activates the
 * door.
 * */

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/TriggerVolume.h"
#include "OpenDoor.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDINGESCAPE_API UOpenDoor : public UActorComponent
{
	GENERATED_BODY()

public:	
	UOpenDoor();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
				//Called every frame

	void OpenDoor(float DeltaTime);

	void CloseDoor(float DeltaTime);

	// Calculates the total mass of objects within the trigger volume. 
	float TotalMassInTrigVol() const;

private:

	float CurrentYaw, InitialYaw;
	FRotator DoorRotation;

	UPROPERTY(EditAnywhere)
	float DoorAngle = 90.0f;

	UPROPERTY(EditAnywhere)
	float OpenSpeed = 1.0f;

	UPROPERTY(EditAnywhere)
	float CloseSpeed = 3.0f;

	UPROPERTY(EditAnywhere)
	float CloseDelay = 3.0f;
	float CloseTime = 0.0f;

	//Amount of total mass needed to trigger door opening
	UPROPERTY(EditAnywhere)
	float TriggerMass = 0.0f;

	UPROPERTY(EditAnywhere)
	ATriggerVolume* PressurePlate;

	UPROPERTY(EditAnywhere)
	AActor* ActorThatEnters;
};
