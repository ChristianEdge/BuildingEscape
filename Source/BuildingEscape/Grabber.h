// Copyright Christian Edge, 2020 

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h" // includes InputComponent.h
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "Grabber.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDINGESCAPE_API UGrabber : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGrabber();

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	/*
	FVector ViewPointLocation = {0.0f, 0.0f, 0.0f};
	FRotator ViewPointRotation = {0.0f, 0.0f, 0.0f};
	*/

	UPROPERTY(EditAnywhere)
	float Reach = 230.0f;

	UPROPERTY()
	UPhysicsHandleComponent* PhysicsHandle = nullptr;
	UPROPERTY()
	UInputComponent* InputComponent = nullptr;

	// Check if there is a valid InputComponent attached to this actor
	void SetupInputComponent();
	// Check if there is a valid Physics Handle attached to this actor
	void CheckPhysicsHandle();

	// Primary function. Grab onto a movable object in the world.
	void Grab();
	// Primary function. Release the object you are holding.
	void Release();

	// Returns the first actor within (float) Reach that has a Physics Body
	FHitResult GetFirstPhysicsBodyInReach() const; 

	// Returns the end of the players reach
	FVector GetReach() const;

	FVector GetParentWorldPos() const;
	FRotator GetParentWorldRot() const;
};
