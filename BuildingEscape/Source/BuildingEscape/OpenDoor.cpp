// Fill out your copyright notice in the Description page of Project Settings.

#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "OpenDoor.h"

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
	
	OpenActor = GetWorld()->GetFirstPlayerController()->GetPawn();
}


// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	RotateDoor();
}

void UOpenDoor::RotateDoor() {
	if (PressurePlate->IsOverlappingActor(OpenActor)) {
		OpenDoor();
	}
}

void UOpenDoor::OpenDoor() {
	AActor* Owner = GetOwner();
	FRotator Rotation = Owner->GetActorRotation();
	FRotator NewRotation = FRotator(.0f, -60.f, .0f);

	UE_LOG(LogTemp, Warning, TEXT("The rotaion of actor %s is %s"), *Owner->GetName(), *Rotation.ToString());

	Owner->SetActorRotation(NewRotation);
}