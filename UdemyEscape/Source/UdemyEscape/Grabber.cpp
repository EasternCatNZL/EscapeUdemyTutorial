// Fill out your copyright notice in the Description page of Project Settings.

#include "Grabber.h"
#include "DrawDebugHelpers.h"
#include "Runtime/Engine/Public/WorldCollision.h"

#define OUT

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();

	// look for attached physics handle
	physicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (physicsHandle) {

	}
	else {
		UE_LOG(LogTemp, Error, TEXT("%s physics handle component not found"), *(GetOwner()->GetName()));
	}

	inputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	if (physicsHandle) {
		UE_LOG(LogTemp, Warning, TEXT("%s input component found"));
		inputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("%s input component not found"), *(GetOwner()->GetName()));
	}
}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...

	//get view point
	FVector playerViewPointLocation;
	FRotator playerViewPointRotation;
	//sets viewpoint location and viewpwoint rotation
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(OUT playerViewPointLocation, OUT playerViewPointRotation);

	//debug logging location and rotation
	/*UE_LOG(LogTemp, Warning, TEXT("Location: %s, Rotation: %s"), *playerViewPointLocation.ToString(), *playerViewPointRotation.ToString());*/

	FVector lineTraceEnd = playerViewPointLocation + playerViewPointRotation.Vector() * reach;

	//ray cast forward reach distance
	DrawDebugLine(GetWorld(), playerViewPointLocation, lineTraceEnd, FColor(255, 0, 0), false, 0.0f, 0.0f, 10.0f);

	//set up query params
	FCollisionQueryParams traceParameters(FName(TEXT("")), false, GetOwner());

	FHitResult lineHit;
	GetWorld()->LineTraceSingleByObjectType(OUT lineHit, playerViewPointLocation, lineTraceEnd, FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody), traceParameters);
	//see what is hit
	AActor* actorHit = lineHit.GetActor();
	
	if (actorHit) {
		UE_LOG(LogTemp, Warning, TEXT("Line trace hit: %s"), *(actorHit->GetName()));
	}
}

void UGrabber::Grab()
{
	UE_LOG(LogTemp, Warning, TEXT("Grab pressed"));
}

