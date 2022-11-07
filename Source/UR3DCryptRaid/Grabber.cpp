// Fill out your copyright notice in the Description page of Project Settings.


#include "Grabber.h"
#include "Engine/World.h"

UGrabber::UGrabber()
{
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();

	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();

	if (PhysicsHandle != nullptr)
	{
		FString PhysicsHandleName = PhysicsHandle->GetName();
		UE_LOG(LogTemp, Display, TEXT("Physics Handle Name: %s"), *PhysicsHandleName);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Physics Handle could not be found!  Grab functionality will not work!"));
	}
}

void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	UpdateHoldPosition();
}

void UGrabber::UpdateHoldPosition()
{
	if (PhysicsHandle == nullptr) { return; }

	FVector HoldPosition = GetComponentLocation() + GetForwardVector() * HoldOffset;
	PhysicsHandle->SetTargetLocationAndRotation(HoldPosition, GetComponentRotation());
}

void UGrabber::Grab()
{
	if (PhysicsHandle == nullptr) { return; }

	if (DebugEnabled) { UE_LOG(LogTemp, Display, TEXT("Initiated grabber")); }

	FVector StartPoint;
	FVector EndPoint;
	GetGrabPoints(StartPoint, EndPoint);
	if (DebugEnabled)
	{
		DrawDebugLine(GetWorld(), StartPoint, EndPoint, FColor::Red, false, 0.1);
		DrawDebugSphere(GetWorld(), EndPoint, 10, 10, FColor::Blue, false, 5);
	}

	CastToGrab(StartPoint, EndPoint);
}

void UGrabber::Release()
{
	if (DebugEnabled) { UE_LOG(LogTemp, Display, TEXT("Released grabber")); }
}

void UGrabber::GetGrabPoints(FVector& OutStartPoint, FVector& OutEndPoint)
{
	AActor* Owner = GetOwner();
	OutStartPoint = Owner->GetActorLocation();
	OutEndPoint = GetComponentLocation() + GetForwardVector() * MaxGrabDistance;
}

void UGrabber::CastToGrab(const FVector& StartPoint, const FVector& EndPoint)
{
	FCollisionShape Sphere = FCollisionShape::MakeSphere(GrabRadius);
	FHitResult HitResult;

	bool HasHit = false;
	HasHit = GetWorld()->SweepSingleByChannel(HitResult,
		StartPoint, EndPoint,
		FQuat::Identity,
		ECC_GameTraceChannel2,
		Sphere
	);

	if (HasHit)
	{
		AActor* HitActor = HitResult.GetActor();
		FString HitActorName = HitActor->GetActorNameOrLabel();

		if (DebugEnabled) 
		{
			DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, 10, 10, FColor::Green, false, 5);
			UE_LOG(LogTemp, Display, TEXT("Found a grabbable object:  %s"), *HitActorName);
		}

		PhysicsHandle->GrabComponentAtLocationWithRotation(
			HitResult.GetComponent(),
			NAME_None,
			HitResult.ImpactPoint,
			GetComponentRotation()
		);
	}
	else
	{
		if (DebugEnabled) { UE_LOG(LogTemp, Display, TEXT("Nothing to grab")); }
	}
}

