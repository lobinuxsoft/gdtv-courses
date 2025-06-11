// Fill out your copyright notice in the Description page of Project Settings.


#include "KillEmAllGameMode.h"

void AKillEmAllGameMode::PawnKilled(APawn* PawnKilled)
{
	Super::PawnKilled(PawnKilled);

	if (GEngine)
		GEngine->AddOnScreenDebugMessage(0, 5, FColor::Red, FString::Printf(TEXT("Player: %s is DEAD!!!"), *PawnKilled->GetActorNameOrLabel()));
}
