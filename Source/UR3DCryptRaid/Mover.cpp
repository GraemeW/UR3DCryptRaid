// Fill out your copyright notice in the Description page of Project Settings.


#include "Mover.h"
#include "Math/UnrealMathUtility.h"

// Sets default values for this component's properties
UMover::UMover()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UMover::BeginPlay()
{
	Super::BeginPlay();

	OriginalLocation = GetOwner()->GetActorLocation();
	TargetLocation = FVector(OriginalLocation + UMover::MoveOffset);
	Speed = MoveOffset.Length() / MoveTime;
}


// Called every frame
void UMover::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	AActor* Owner = GetOwner();
	FVector CurrentLocation = Owner->GetActorLocation();
	FVector NewLocation;
	if (ShouldMove)
	{
		NewLocation = FMath::VInterpConstantTo(CurrentLocation, TargetLocation, DeltaTime, Speed);
	}
	else
	{
		NewLocation = FMath::VInterpConstantTo(CurrentLocation, OriginalLocation, DeltaTime, Speed);
	}
	Owner->SetActorLocation(NewLocation);
}

void UMover::SetShouldMove(bool enable)
{
	ShouldMove = enable;
}

void UMover::ToggleShouldMove()
{
	ShouldMove = !ShouldMove;
}
