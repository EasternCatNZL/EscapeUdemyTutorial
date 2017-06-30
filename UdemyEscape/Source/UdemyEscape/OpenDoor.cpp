// Fill out your copyright notice in the Description page of Project Settings.

#include "OpenDoor.h"
#include "Runtime/Engine/Classes/Engine/TriggerVolume.h"
#include "Gameframework/Actor.h"

// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();

	// ...
	actorThatOpens =  GetWorld()->GetFirstPlayerController()->GetPawn();
	owner = GetOwner();
}


// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
	//if actor that can activate pressure plate steps into collision
	if (pressurePlate->IsOverlappingActor(actorThatOpens)) {
		OpenTheDoor();
		lastDoorOpenTime = GetWorld()->GetTimeSeconds();
	}

	//check if time to close door
	if (lastDoorOpenTime + doorCloseDelay <= GetWorld()->GetTimeSeconds()) {
		CloseTheDoor();
	}
}

void UOpenDoor::OpenTheDoor()
{
	//set the door rotation
	owner->SetActorRotation(FRotator(0.0f, openAngle, 0.0f));
}

void UOpenDoor::CloseTheDoor() {
	//set the door rotation
	owner->SetActorRotation(FRotator(0.0f, 0.0f, 0.0f));
}