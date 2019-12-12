// Fill out your copyright notice in the Description page of Project Settings.

#include "OpenDoor.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "Engine/StaticMesh.h"
#include "Components/PrimitiveComponent.h"

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

	Owner = GetOwner();
}


// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	RotateDoor();
}

void UOpenDoor::RotateDoor() {
	if (GetTotalMassOnPlate()>TriggerMass) {
		OnOpen.Broadcast();
	}
	else {
		OnClose.Broadcast();
	}
}


float UOpenDoor::GetTotalMassOnPlate() {
	float Mass = 0;
	TArray<AActor*> OverlappedActor;
	if (!PressurePlate) {
		UE_LOG(LogTemp, Error, TEXT("Pressure plate missing"));
		return Mass;
	}
	PressurePlate->GetOverlappingActors(OUT OverlappedActor);
	for (const AActor* Actor : OverlappedActor) {
		Mass += Actor->FindComponentByClass<UPrimitiveComponent>()->CalculateMass();
	}
	return Mass;
}
