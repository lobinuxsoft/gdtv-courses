// Fill out your copyright notice in the Description page of Project Settings.


#include "Mover.h"

// Sets default values for this component's properties
UMover::UMover()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	UE_LOG(LogTemp, Display, TEXT("Mover is created!"));
}


// Called when the game starts
void UMover::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(LogTemp, Display, TEXT("Mover is beginning!"));
}


// Called every frame
void UMover::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	AActor* Owner = GetOwner();

	UE_LOG(LogTemp, Display, TEXT("Owner Actor Name: %s"), *Owner->GetActorNameOrLabel());

	FVector Location = Owner->GetActorLocation();

	UE_LOG(LogTemp, Display, TEXT("Owner Actor Location: %s"), *Location.ToCompactString());
}