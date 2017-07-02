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

	FindPhysicsHandleComponent();

	FindInputComponent();
}

void UGrabber::FindPhysicsHandleComponent() {
	// look for attached physics handle
	physicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (physicsHandle) {

	}
	else {
		UE_LOG(LogTemp, Error, TEXT("%s physics handle component not found"), *(GetOwner()->GetName()));
	}
}


void UGrabber::FindInputComponent() {
	inputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	if (physicsHandle) {
		UE_LOG(LogTemp, Warning, TEXT("%s input component found"));
		inputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		inputComponent->BindAction("Grab", IE_Released, this, &UGrabber::Release);
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("%s input component not found"), *(GetOwner()->GetName()));
	}
}

const FHitResult UGrabber::GetFirstPhysicsBodyInReach()
{
	//get view point
	FVector playerViewPointLocation;
	FRotator playerViewPointRotation;
	//sets viewpoint location and viewpwoint rotation
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(OUT playerViewPointLocation, OUT playerViewPointRotation);

	FVector lineTraceEnd = playerViewPointLocation + playerViewPointRotation.Vector() * reach;

	//set up query params
	FCollisionQueryParams traceParameters(FName(TEXT("")), false, GetOwner());

	FHitResult lineHit;
	GetWorld()->LineTraceSingleByObjectType(OUT lineHit, playerViewPointLocation, lineTraceEnd, FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody), traceParameters);
	//see what is hit
	AActor* actorHit = lineHit.GetActor();

	if (actorHit) {
		UE_LOG(LogTemp, Warning, TEXT("Line trace hit: %s"), *(actorHit->GetName()));
	}

	return lineHit;
}

// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...

	//if physics handle attached, move object we're holding

	
}

void UGrabber::Grab()
{
	UE_LOG(LogTemp, Warning, TEXT("Grab pressed"));

	//try and reach any actors with physics body collision
	GetFirstPhysicsBodyInReach();
	//if hit, attach physics handle
}

void UGrabber::Release()
{
	UE_LOG(LogTemp, Warning, TEXT("Grab released"));

	//if holding something, release handle
}

