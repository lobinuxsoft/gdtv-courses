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

	// Set up inputs for this component
	ACryptRiderCharacter* Character = Cast<ACryptRiderCharacter>(GetOwner());

	// Set up action bindings
	if (APlayerController* PlayerController = Cast<APlayerController>(Character->GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
			Subsystem->AddMappingContext(GrabberMappingContext, 1);

		if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerController->InputComponent))
		{
			EnhancedInputComponent->BindAction(GrabAction, ETriggerEvent::Started, this, &UGrabber::Grab);
			EnhancedInputComponent->BindAction(GrabAction, ETriggerEvent::Completed, this, &UGrabber::Release);
		}
	}

	UPhysicsHandleComponent* PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	
	if (PhysicsHandle && GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Cyan, FString::Printf(TEXT("Hit Actor Name: %s"), *PhysicsHandle->GetName()));
}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UGrabber::Grab()
{
	FVector StartPoint = GetComponentLocation();
	FVector EndPoint = StartPoint + GetForwardVector() * MaxGrabDistance;
	DrawDebugLine(GetWorld(), StartPoint, EndPoint, FColor::Red, false, 5);
	DrawDebugSphere(GetWorld(), EndPoint, 10, 10, FColor::Blue, false, 5);
	
	FCollisionShape Sphere = FCollisionShape::MakeSphere(GrabRadius);
	FHitResult HitResult;
	bool HasHit = GetWorld()->SweepSingleByChannel(HitResult, StartPoint, EndPoint, FQuat::Identity, ECC_GameTraceChannel2, Sphere);

	if (HasHit)
	{
		DrawDebugLine(GetWorld(), HitResult.TraceStart, HitResult.TraceEnd, FColor::Green, false, 5);
		DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, 10, 10, FColor::Green, false, 5);
		DrawDebugSphere(GetWorld(), HitResult.Location, GrabRadius, 10, FColor::Yellow, false, 5);

		if (GEngine)
			GEngine->AddOnScreenDebugMessage(
				-1,
				3.0f,
				FColor::Yellow,
				FString::Printf(TEXT("Hit Actor Name: %s"), *HitResult.GetActor()->GetName())
			);
	}
}

void UGrabber::Release()
{
	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Yellow, TEXT("RELEASE"));
}
