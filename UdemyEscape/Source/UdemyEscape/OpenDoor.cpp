// Fill out your copyright notice in the Description page of Project Settings.

#include "OpenDoor.h"
#include "Runtime/Engine/Classes/Engine/TriggerVolume.h"
#include "Gameframework/Actor.h"

#define OUT

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


	owner = GetOwner();
}


// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
	//if actor that can activate pressure plate steps into collision

	if (GetTotalMassOfActorsOnPlate() > weightNeeded) {
		onOpenRequest.Broadcast();
	}
	else {
		onCloseRequest.Broadcast();
	}
}

float UOpenDoor::GetTotalMassOfActorsOnPlate() {
	float totalMass = 0.0f;

	//find overlapping actors, interate through and add masses
	TArray<AActor*> overlappingActors;
	if (!pressurePlate) {
		UE_LOG(LogTemp, Warning, TEXT("Pressure plate not set"));
		return totalMass;
	}

	pressurePlate->GetOverlappingActors(OUT overlappingActors);
	for (const auto* i : overlappingActors) {
		totalMass += i->FindComponentByClass<UPrimitiveComponent>()->GetMass();
		UE_LOG(LogTemp, Warning, TEXT("%s on pressure plate"), *i->GetName());
	}

	return totalMass;
}