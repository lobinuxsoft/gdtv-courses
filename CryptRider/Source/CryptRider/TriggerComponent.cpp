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

	if(AActor* Actor = GetAcceptableActor())
	{
		if( UPrimitiveComponent* Component = Cast<UPrimitiveComponent>(Actor->GetRootComponent()))
			Component->SetSimulatePhysics(false);
		
		Actor->AttachToComponent(this->GetAttachParent(), FAttachmentTransformRules::SnapToTargetIncludingScale, "Socket");
		Mover->SetShouldMove(true);
	}
	else
		Mover->SetShouldMove(false);
}

void UTriggerComponent::SetMover(UMover* NewMover) { Mover = NewMover; }

AActor* UTriggerComponent::GetAcceptableActor() const
{
	TArray<AActor*> Actors;
	GetOverlappingActors(Actors);

	for (AActor* Actor : Actors)
		if (Actor->ActorHasTag(TargetTag) && !Actor->ActorHasTag("Grabbed"))
			return Actor;

	return nullptr;
}
