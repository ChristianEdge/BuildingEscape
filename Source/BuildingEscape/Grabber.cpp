// Copyright Christian Edge, 2020 

#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/PlayerController.h"
#include "Grabber.h"

#define OUT


// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}

// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();

	CheckPhysicsHandle();

	SetupInputComponent();
}

// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	//If a physics handle is attached
	if (PhysicsHandle->GrabbedComponent)
	{
		//Move the object we are currently grabbing
		PhysicsHandle->SetTargetLocation(GetReach());
	}
}

void UGrabber::Grab()
{
	FHitResult HitResult =  GetFirstPhysicsBodyInReach();

	if (HitResult.GetActor())
	{
		UPrimitiveComponent* ComponentToGrab = HitResult.GetComponent();

		PhysicsHandle->GrabComponentAtLocation(
			ComponentToGrab,
			NAME_None,
			GetReach()
		);
	}
}

void UGrabber::Release()
{
	PhysicsHandle->ReleaseComponent();
}

void UGrabber::SetupInputComponent()
{
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	if (!InputComponent->HasBindings())
	{
		UE_LOG(LogTemp, Error, TEXT("Actor %s has no bindings for InputComponent!!"),
			*GetOwner()->GetName());
	}
	if (InputComponent)
	{
		InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		InputComponent->BindAction("Grab", IE_Released, this, &UGrabber::Release);
	}
}

//Checks for an attached Physics Handle Component
void UGrabber::CheckPhysicsHandle()
{
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (PhysicsHandle == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Actor %s has no PhysicsHandleComponent!!!"),
			*GetOwner()->GetName());
	}
}

FHitResult UGrabber::GetFirstPhysicsBodyInReach() const
{
	/*
	DrawDebugLine(
		GetWorld(),
		GetParentWorldPos(),
		GetReach(),
		FColor(244, 0, 0),
		false,
		0.0f,
		0,
		10.0f
	);
	*/

	FHitResult TargetHit;

	FCollisionQueryParams HitParams(FName(TEXT("")), false, GetOwner());
	
	GetWorld()->LineTraceSingleByObjectType(
		OUT TargetHit, 
		GetParentWorldPos(),
		GetReach(),
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		HitParams
	);

	/*
	//AActor* ActorThatwasHit = TargetHit.GetActor(); //or
	if (TargetHit.GetActor())
	{
		UE_LOG(LogTemp, Warning, TEXT("Actor hit : %s"), 
		*TargetHit.GetActor()->GetName()
		);	
	}
	*/

	return TargetHit;
}

FVector UGrabber::GetReach() const
{
	FVector ViewPointLocation = GetOwner()->GetActorLocation();
	FRotator ViewPointRotation = GetOwner()->GetActorRotation(); 
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT ViewPointLocation,
		OUT ViewPointRotation
	);
	return ViewPointLocation + (ViewPointRotation.Vector() * Reach);
}

FVector UGrabber::GetParentWorldPos() const
{
	FVector ViewPointLocation = GetOwner()->GetActorLocation();
	FRotator ViewPointRotation = GetOwner()->GetActorRotation(); 
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT ViewPointLocation,
		OUT ViewPointRotation
	);
	return ViewPointLocation;
}