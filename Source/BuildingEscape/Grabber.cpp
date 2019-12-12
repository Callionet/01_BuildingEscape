// Fill out your copyright notice in the Description page of Project Settings.

#include "Grabber.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "Components/PrimitiveComponent.h"
#include "DrawDebugHelpers.h"


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
	Initialize();
	GrabControl();
}

// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (PhysicHandler->GetGrabbedComponent()) {
		PhysicHandler->SetTargetLocation(LineTraceEnd());
	}	
}

FVector UGrabber::LineTraceStart() const {
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewPointLocation,
		OUT PlayerViewPointRotation);

	return PlayerViewPointLocation;
}

FVector UGrabber::LineTraceEnd() const {
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewPointLocation,
		OUT PlayerViewPointRotation);

	return PlayerViewPointLocation + PlayerViewPointRotation.Vector() * Reach;
}

FHitResult UGrabber::RayCasting() const {
	//setup query parameter
	FCollisionQueryParams TraceParameter(FName(TEXT("Nothing")), false, GetOwner());
	//raycasting
	FHitResult HitItem;

	if (GetWorld()->LineTraceSingleByObjectType(
		OUT HitItem,
		LineTraceStart(),
		LineTraceEnd(),
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		TraceParameter)) {
	}

	return HitItem;
}

void UGrabber::Initialize() {
	UE_LOG(LogTemp, Warning, TEXT("Grabber initiate"));

	PhysicHandler = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (!PhysicHandler) {
		UE_LOG(LogTemp, Error, TEXT("%s Physics handler not found."), *GetOwner()->GetName());
	}

	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	if (!InputComponent) {
		UE_LOG(LogTemp, Error, TEXT("%s InputComponent not found."), *GetOwner()->GetName());
	}
}

void UGrabber::GrabControl() {
	if (InputComponent) {
		InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		InputComponent->BindAction("Grab", IE_Released, this, &UGrabber::Release);
	}
}

void UGrabber::Release() {
	if (PhysicHandler->GetGrabbedComponent()) {
		PhysicHandler->ReleaseComponent();
	}
}

void UGrabber::Grab() {
	FHitResult HitResult = RayCasting();
	UPrimitiveComponent* ComponentToGrab = HitResult.GetComponent();

	if (HitResult.GetActor()) {
		PhysicHandler->GrabComponent(
			ComponentToGrab,
			NAME_None,
			ComponentToGrab->GetOwner()->GetActorLocation(),
			true);
	}
}
