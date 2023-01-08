// Fill out your copyright notice in the Description page of Project Settings.


#include "Grabber.h"
#include "Engine/World.h"

UGrabber::UGrabber()
{
	PrimaryComponentTick.bCanEverTick = true;
}
const FName UGrabber::TagGrabbed = "Grabbed";

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

	if (PhysicsHandle->GetGrabbedComponent() != nullptr)
	{
		FVector HoldPosition = GetComponentLocation() + GetForwardVector() * HoldOffset;
		PhysicsHandle->SetTargetLocationAndRotation(HoldPosition, GetComponentRotation());
	}
}

void UGrabber::Grab()
{
	if (PhysicsHandle == nullptr) { return; }

	if (DebugEnabled) { UE_LOG(LogTemp, Display, TEXT("Initiated grabber")); }

	FHitResult HitResult;
	if (GetGrabbableInReach(HitResult))
	{
		UpdateGrabTag(true, HitResult.GetActor());
		GrabObject(HitResult);
	}
	else
	{
		if (DebugEnabled) { UE_LOG(LogTemp, Display, TEXT("Nothing to grab")); }
	}
}

void UGrabber::Release()
{
	if (DebugEnabled) { UE_LOG(LogTemp, Display, TEXT("Released grabber")); }

	if (GrabbedComponent != nullptr)
	{ 
		AActor* GrabbedActor = GrabbedComponent->GetOwner();
		if (GrabbedActor != nullptr) { UpdateGrabTag(false, GrabbedActor); }

		GrabbedComponent->WakeAllRigidBodies();
		GrabbedComponent = nullptr; 
	}
	if (PhysicsHandle->GetGrabbedComponent() != nullptr)
	{ 
		PhysicsHandle->ReleaseComponent(); 
	}
}

void UGrabber::UpdateGrabTag(bool enable, AActor* GrabbedActor)
{
	if (enable)
	{
		GrabbedActor->Tags.Remove(TagGrabbed);
		GrabbedActor->Tags.Add(TagGrabbed);
	}
	else
	{
		GrabbedActor->Tags.Remove(TagGrabbed);
	}
}

void UGrabber::GetGrabPoints(FVector& OutStartPoint, FVector& OutEndPoint)
{
	AActor* Owner = GetOwner();
	OutStartPoint = Owner->GetActorLocation();
	OutEndPoint = GetComponentLocation() + GetForwardVector() * MaxGrabDistance;
}

bool UGrabber::GetGrabbableInReach(FHitResult& HitResult)
{
	FVector StartPoint;
	FVector EndPoint;
	GetGrabPoints(StartPoint, EndPoint);
	if (DebugEnabled)
	{
		DrawDebugLine(GetWorld(), StartPoint, EndPoint, FColor::Red, false, 0.1);
		DrawDebugSphere(GetWorld(), EndPoint, 10, 10, FColor::Blue, false, 5);
	}

	FCollisionShape Sphere = FCollisionShape::MakeSphere(GrabRadius);

	return GetWorld()->SweepSingleByChannel(HitResult,
		StartPoint, EndPoint,
		FQuat::Identity,
		ECC_GameTraceChannel2,
		Sphere
	);
}

void UGrabber::GrabObject(const FHitResult& HitResult)
{
	if (&HitResult == nullptr) { return; }

	AActor* HitActor = HitResult.GetActor();
	FString HitActorName = HitActor->GetActorNameOrLabel();

	if (DebugEnabled)
	{
		DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, 10, 10, FColor::Green, false, 5);
		UE_LOG(LogTemp, Display, TEXT("Found a grabbable object:  %s"), *HitActorName);
	}

	GrabbedComponent = HitResult.GetComponent();
	if (GrabbedComponent == nullptr) { return; }

	GrabbedComponent->WakeAllRigidBodies();
	PhysicsHandle->GrabComponentAtLocationWithRotation(
		GrabbedComponent,
		NAME_None,
		HitResult.ImpactPoint,
		GetComponentRotation()
	);
}

