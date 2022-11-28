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

	TSet <AActor*> OverlappingObjects;
	GetOverlappingActors(OverlappingObjects);

	for (AActor* OverlappingObject : OverlappingObjects)
	{
		UE_LOG(LogTemp, Display, TEXT("Overlapping component found:  %s"), *OverlappingObject->GetActorNameOrLabel());
	}
}
