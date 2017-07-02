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
;

	//set up query params
	FCollisionQueryParams traceParameters(FName(TEXT("")), false, GetOwner());

	FHitResult lineHit;
	GetWorld()->LineTraceSingleByObjectType(OUT lineHit, GetReachLineStart(), GetReachLineEnd(), FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody), traceParameters);

	return lineHit;
}

FVector UGrabber::GetReachLineEnd()
{
	//get view point
	FVector playerViewPointLocation;
	FRotator playerViewPointRotation;
	//sets viewpoint location and viewpwoint rotation
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(OUT playerViewPointLocation, OUT playerViewPointRotation);

	return playerViewPointLocation + playerViewPointRotation.Vector() * reach;
	
}

FVector UGrabber::GetReachLineStart()
{
	//get view point
	FVector playerViewPointLocation;
	FRotator playerViewPointRotation;
	//sets viewpoint location and viewpwoint rotation
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(OUT playerViewPointLocation, OUT playerViewPointRotation);

	return playerViewPointLocation;

}

// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	//if physics handle attached, move object we're holding
	if (physicsHandle->GrabbedComponent) {
		//move object we're holding
		physicsHandle->SetTargetLocation(GetReachLineEnd());
		
	}

}

void UGrabber::Grab()
{
	//try and reach any actors with physics body collision
	auto hitResult = GetFirstPhysicsBodyInReach();
	auto componentToGrab = hitResult.GetComponent();
	auto actorHit = hitResult.GetActor();

	if (actorHit) {
		physicsHandle->GrabComponentAtLocation(componentToGrab, NAME_None, componentToGrab->GetOwner()->GetActorLocation());
	}
}

void UGrabber::Release()
{
	//if holding something, release handle
	physicsHandle->ReleaseComponent();
}