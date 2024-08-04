// Fill out your copyright notice in the Description page of Project Settings.
#include "TriggerComponent.h"

UTriggerComponent::UTriggerComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UTriggerComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UTriggerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	TArray<AActor*> Actors;
	GetOverlappingActors(Actors);
	
	if (Actors.Num() > 0)
	{
		for (int i = 0; i < Actors.Num(); ++i)
		{
			if(GEngine)
			{
				FString message = FString::Printf(TEXT("Actor: %s"), *Actors[i]->GetActorNameOrLabel());
				GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Orange, message);
			}
		}
	}
}
