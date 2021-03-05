// Copyright Maninmaroon 2020.


#include "OpenDoor.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "Components/PrimitiveComponent.h"
#include "GameFramework/Actor.h"
#include "Grabber.h"

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
	if (!PressurePlate) { 
		UE_LOG(LogTemp, Error, TEXT("%s missing pressure component"), *GetOwner()->GetName());
	}
}

/*
void UOpenDoor::OpenDoor()
{
	//Create a rotator[but see the definition here we removed the variable and directly asigned the value here]
	FRotator NewRotation = FRotator(0.f, -60.f, 0.f); 

	//Set the door rotation
	Owner->SetActorRotation(FRotator(0.f, OpenAngle, 0.f));
}
*/

/*
void UOpenDoor::CloseDoor()
{
	Owner->SetActorRotation(FRotator(0.f, 0.f, 0.f));
}
*/

// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	//Poll the Trigger Volume
	//If the ActorThatOpens is in the volume
	if (GetTotalMassOfActorsOnPlate() > TriggerMass) {
		//OpenDoor();
		//LastDoorOpenTime = GetWorld()->GetTimeSeconds();
		OnOpen.Broadcast();
	}
	else
	{
		OnClose.Broadcast();
	}
}
	/*
	//Check if it's time to close the door
	if (GetWorld()->GetTimeSeconds() - LastDoorOpenTime > DoorCloseDelay) {
		//CloseDoor();

	}*/

float UOpenDoor::GetTotalMassOfActorsOnPlate() {

	float TotalMass = 0.f;

	//Find all the ovelapping Actors
	TArray<AActor*> OverlappingActors;
	if (!PressurePlate) { return TotalMass; }
	PressurePlate->GetOverlappingActors(OUT OverlappingActors);

	//Iterate through them adding masses
	for (const AActor* Actor : OverlappingActors) {
		TotalMass += Actor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
		UE_LOG(LogTemp, Warning, TEXT("%s on pressure plate"), *Actor->GetName());
	}

	return TotalMass;
}