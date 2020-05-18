// Fill out your copyright notice in the Description page of Project Settings.


#include "WorldPosition.h"
#include "GameFramework/Actor.h"

// Sets default values for this component's properties
UWorldPosition::UWorldPosition()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UWorldPosition::BeginPlay()
{
	Super::BeginPlay();

	/*
	// Testing
	FString Hello = TEXT("Hello! An actor has begun play.");
	FString* pHello = &Hello;
	UE_LOG(LogTemp, Warning, TEXT("%s"), *Hello);
	UE_LOG(LogTemp, Warning, TEXT("%s"), **pHello);
	*/
	

	FString MyName = GetOwner()->GetName();
	UE_LOG(LogTemp, Warning, TEXT("My owner's name is %s."), *MyName);

	UE_LOG(LogTemp, Warning, TEXT("My name is %s."), *GetName());

	FString MyPosition = GetOwner()->GetActorLocation().ToString();
	UE_LOG(LogTemp, Warning, TEXT("My world location is %s."), *MyPosition);
}


// Called every frame
void UWorldPosition::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

