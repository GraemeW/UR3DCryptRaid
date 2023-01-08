// Fill out your copyright notice in the Description page of Project Settings.


#include "TriggerComponent.h"


UTriggerComponent::UTriggerComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

// Called when the game starts
void UTriggerComponent::BeginPlay()
{
	Super::BeginPlay();
}


void UTriggerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (triggered) { return; } // Only trigger once

	AActor* AcceptableActor = GetAcceptableActor();
	if (AcceptableActor == nullptr) { return; }

	// Wait for object to be released
	if (CheckForGrabberRelease)
	{
		if (AcceptableActor->Tags.Contains(UGrabber::TagGrabbed)) { return; }
	}

	// When released, enable movement
	UPrimitiveComponent* ActorRoot = Cast<UPrimitiveComponent>(AcceptableActor->GetRootComponent());
	if (Movers.Num() > 0)
	{ 
		triggered = true;

		if (BindObjectToTrigger && ActorRoot != nullptr)
		{
			ActorRoot->SetSimulatePhysics(false);
			AcceptableActor->AttachToComponent(this, FAttachmentTransformRules::KeepWorldTransform);
		}

		for (UMover* Mover : Movers)
		{
			Mover->ToggleShouldMove();
		}
	}
	UE_LOG(LogTemp, Display, TEXT("Unlocked with:  %s"), *AcceptableActor->GetActorNameOrLabel());
}

TArray<UMover*> UTriggerComponent::GetOnTriggerMovers()
{
	TArray<UMover*> OnTriggerMovers;
	for (AActor* TentativeMover : OnTriggerMoverActors)
	{
		UMover* NewMover = TentativeMover->FindComponentByClass<UMover>();
		if (NewMover == nullptr) { continue; }
		OnTriggerMovers.Add(NewMover);
	}
	return OnTriggerMovers;
}

void UTriggerComponent::SetMovers(TArray<UMover*> NewMovers)
{
	Movers = NewMovers;
}

AActor* UTriggerComponent::GetAcceptableActor()
{
	TSet <AActor*> Actors;
	GetOverlappingActors(Actors);
	for (AActor* Actor : Actors)
	{
		if (Actor->ActorHasTag(*TriggerTag)) 
		{
			return Actor;
		}
	}
	return nullptr;
}


