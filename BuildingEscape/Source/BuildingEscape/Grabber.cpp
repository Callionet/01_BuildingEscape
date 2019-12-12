// Fill out your copyright notice in the Description page of Project Settings.

#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "Grabber.h"
#include "DrawDebugHelpers.h"


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
	Initialize();
	GrabControl();
}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	RayCasting();
}

void UGrabber::RayCasting() {
	//get viewpoint
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewPointLocation,
		OUT PlayerViewPointRotation);


	if (GetWorld()->GetTimeSeconds() - TimeCount >= Delay) {
		UE_LOG(LogTemp, Warning, TEXT("Location: %s, Rotation: %s"), *PlayerViewPointLocation.ToString(), *PlayerViewPointRotation.ToString());
		TimeCount = GetWorld()->GetTimeSeconds();
	}

	//draw red trace
	FVector  LineTraceEnd = PlayerViewPointLocation + PlayerViewPointRotation.Vector() * Reach;
	DrawDebugLine(GetWorld(), PlayerViewPointLocation, LineTraceEnd, FColor(255, 0, 0), false, 0.f, 0.f, 5.f);

	//setup query parameter
	FCollisionQueryParams TraceParameter(FName(TEXT("Nothing")), false, GetOwner());
	//raycasting
	FHitResult HitItem;

	if (GetWorld()->LineTraceSingleByObjectType(
		OUT HitItem,
		PlayerViewPointLocation,
		LineTraceEnd,
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		TraceParameter)) {

		UE_LOG(LogTemp, Warning, TEXT("Hit item: %s"), *HitItem.GetActor()->GetName());
	}
}

void UGrabber::Initialize() {
	UE_LOG(LogTemp, Warning, TEXT("Grabber initiate"));

	PhysicHandler = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (!PhysicHandler) {
		UE_LOG(LogTemp, Error, TEXT("%s Physics handler not found."), *GetOwner()->GetName());
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("%s Physics handler found."), *GetOwner()->GetName());
	}

	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	if (!InputComponent) {
		UE_LOG(LogTemp, Error, TEXT("%s InputComponent not found."), *GetOwner()->GetName());
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("%s InputComponent found."), *GetOwner()->GetName());
	}
}

void UGrabber::GrabControl() {
	if (InputComponent) {
		InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		InputComponent->BindAction("Grab", IE_Released, this, &UGrabber::Release);
	}
}

void UGrabber::Release() {
	UE_LOG(LogTemp, Warning, TEXT("Release"));
}

void UGrabber::Grab(){
	UE_LOG(LogTemp, Warning, TEXT("Grab"));
}
