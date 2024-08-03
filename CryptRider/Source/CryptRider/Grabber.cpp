// Fill out your copyright notice in the Description page of Project Settings.

#include "Grabber.h"

#include "CryptRiderCharacter.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"

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

	// Set up action bindings
	if (APlayerController* PlayerController = Cast<APlayerController>(Cast<ACryptRiderCharacter>(GetOwner())->GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
			Subsystem->AddMappingContext(GrabberMappingContext, 1);

		if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerController->InputComponent))
		{
			EnhancedInputComponent->BindAction(GrabAction, ETriggerEvent::Started, this, &UGrabber::Grab);
			EnhancedInputComponent->BindAction(GrabAction, ETriggerEvent::Completed, this, &UGrabber::Release);
		}
	}
}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (UPhysicsHandleComponent* PhysicsHandle = GetPhysicsHandle())
	{
		if (PhysicsHandle->GetGrabbedComponent() != nullptr)
		{
			FVector TargetLocation = GetComponentLocation() + GetForwardVector() * HoldDistance;
			PhysicsHandle->SetTargetLocationAndRotation(TargetLocation, GetComponentRotation());
		}
	}
}

void UGrabber::Grab()
{
	if (UPhysicsHandleComponent* PhysicsHandle = GetPhysicsHandle())
	{
		FHitResult HitResult;

		if (GetGrabbableInReach(HitResult))
		{
			UPrimitiveComponent* HitComponent = HitResult.GetComponent();
			HitComponent->WakeAllRigidBodies();
			PhysicsHandle->GrabComponentAtLocationWithRotation(HitComponent, NAME_None, HitResult.ImpactPoint, GetComponentRotation());
		}
	}
}

void UGrabber::Release()
{
	if (UPhysicsHandleComponent* PhysicsHandle = GetPhysicsHandle())
	{
		if (PhysicsHandle->GetGrabbedComponent() != nullptr)
			PhysicsHandle->ReleaseComponent();
	}
}

UPhysicsHandleComponent* UGrabber::GetPhysicsHandle() const
{
	UPhysicsHandleComponent* Result = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();

	if (Result == nullptr)
		UE_LOG(LogTemp, Error, TEXT("Grabber requires a UPhysicsHandleComponent."));

	return Result;
}

bool UGrabber::GetGrabbableInReach(FHitResult& OutHitResult) const
{
	FVector StartPoint = GetComponentLocation();
	FVector EndPoint = StartPoint + GetForwardVector() * MaxGrabDistance;
	// DrawDebugLine(GetWorld(), StartPoint, EndPoint, FColor::Red, false, 5);
	// DrawDebugSphere(GetWorld(), EndPoint, 10, 10, FColor::Blue, false, 5);

	FCollisionShape Sphere = FCollisionShape::MakeSphere(GrabRadius);
	
	return GetWorld()->SweepSingleByChannel(OutHitResult, StartPoint, EndPoint, FQuat::Identity, ECC_GameTraceChannel2, Sphere);
}
