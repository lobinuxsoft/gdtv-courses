// Fill out your copyright notice in the Description page of Project Settings.


#include "Grabber.h"

#include "CryptRiderCharacter.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Math/UnitConversion.h"

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
			EnhancedInputComponent->BindAction(GrabAction, ETriggerEvent::Canceled, this, &UGrabber::Release);
	}
}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	FVector StartPoint = GetComponentLocation();
	FVector EndPoint = StartPoint + GetForwardVector() * MaxGrabDistance;

	FCollisionShape Sphere = FCollisionShape::MakeSphere(GrabRadius);
	FHitResult HitResult;
	bool HasHit = GetWorld()->SweepSingleByChannel(HitResult, StartPoint, EndPoint, FQuat::Identity, ECC_GameTraceChannel2, Sphere);

	if (HasHit)
	{
		DrawDebugLine(GetWorld(), StartPoint, HitResult.ImpactPoint, FColor::Green);
		DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, GrabRadius, 16, FColor::Green);

		UE_LOG(LogTemp, Display, TEXT("Hit Actor Name: %s"), *HitResult.GetActor()->GetName());
	}
	else
	{
		DrawDebugLine(GetWorld(), StartPoint, EndPoint, FColor::Red);
		DrawDebugSphere(GetWorld(), EndPoint, GrabRadius, 16, FColor::Red);
	}
}

void UGrabber::Release()
{
	if(GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Yellow, TEXT("Released grabber"));
	
	UE_LOG(LogTemp, Display, TEXT("Released grabber"));
}
