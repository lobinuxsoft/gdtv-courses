// Fill out your copyright notice in the Description page of Project Settings.


#include "KillEmAllGameMode.h"

void AKillEmAllGameMode::PawnKilled(APawn* PawnKilled)
{
	Super::PawnKilled(PawnKilled);

	if (APlayerController* PlayerController = Cast<APlayerController>(PawnKilled->GetController()))
	{
		PlayerController->GameHasEnded(nullptr, false);
	}
}
