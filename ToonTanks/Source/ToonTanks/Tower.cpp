// Fill out your copyright notice in the Description page of Project Settings.


#include "Tower.h"
#include "Tank.h"
#include "Kismet/GameplayStatics.h"

ATower::ATower()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ATower::BeginPlay()
{
	Super::BeginPlay();

	Tank = Cast<ATank>(UGameplayStatics::GetPlayerPawn(this, 0));
}

void ATower::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (Tank)
	{
		// Find the distance to the Tank
		float Distance = FVector::Distance(GetActorLocation(), Tank->GetActorLocation());
		bool OutOfRange = Distance > FireRange;

		DrawDebugSphere(GetWorld(), GetActorLocation(), FireRange, 16, OutOfRange ? FColor::Red : FColor::Green);
		
		// Check to see if the Tank is in range
		if(OutOfRange)
			return;
		
		// If in range, rotate turret toward Tank
		RotateTurret(Tank->GetActorLocation());
	}
}
