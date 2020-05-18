// Copyright Christian Edge, 2020 

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/TriggerVolume.h"
#include "SwingingDoor_InterpConstTo.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDINGESCAPE_API USwingingDoor_InterpConstTo : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USwingingDoor_InterpConstTo();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void OpenDoor(float DeltaTime);

	void CloseDoor(float DeltaTime);

private:

	float CurrentYaw, InitialYaw;
	FRotator DoorRotation;

	UPROPERTY(EditAnywhere)
	float TargetYaw = 90.0f;

	UPROPERTY(EditAnywhere)
	float OpenSpeed = 45.0f;

	UPROPERTY(EditAnywhere)
	float OpenSecs = 3.0f;

	UPROPERTY(EditAnywhere)
	float CloseSpeed = 120.0f;

	UPROPERTY(EditAnywhere)
	float CloseDelay = 3.0f;
	float CloseTime = 0.0f;	

	UPROPERTY(EditAnywhere)
	ATriggerVolume* PressurePlate;

	UPROPERTY(EditAnywhere)
	AActor* ActorThatEnters;

};
