// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterAIController.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"

void AShooterAIController::BeginPlay()
{
	Super::BeginPlay();

	if (AIBehavior)
	{
		RunBehaviorTree(AIBehavior);
		
		GetBlackboardComponent()->SetValueAsVector(StartLocationKey, GetPawn()->GetActorLocation());
	}
}

void AShooterAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0))
	{
		if (LineOfSightTo(PlayerPawn))
		{
			GetBlackboardComponent()->SetValueAsVector(PlayerLocationKey, PlayerPawn->GetActorLocation());
			GetBlackboardComponent()->SetValueAsVector(LastKnownPlayerLocationKey, PlayerPawn->GetActorLocation());
		}
		else
		{
			GetBlackboardComponent()->ClearValue(PlayerLocationKey);
		}
	}
}
