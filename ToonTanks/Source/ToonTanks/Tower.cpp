// Fill out your copyright notice in the Description page of Project Settings.


#include "Tower.h"
#include "Tank.h"
#include "Kismet/GameplayStatics.h"

void ATower::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	// Check to see if the Tank is in range
	if (InFireRange())
	{
		// If in range, rotate turret toward Tank
		RotateTurret(Tank->GetActorLocation());
	}
}

void ATower::BeginPlay()
{
	Super::BeginPlay();

	Tank = Cast<ATank>(UGameplayStatics::GetPlayerPawn(this, 0));

	GetWorldTimerManager().SetTimer(FireRateTimerHandle, this, &ATower::CheckFireCondition, FireRate, true);
}

void ATower::CheckFireCondition()
{
	if (InFireRange())
		Fire();
}

bool ATower::InFireRange()
{
	if (Tank)
	{
		float Distance = FVector::Distance(GetActorLocation(), Tank->GetActorLocation());
		bool OutOfRange = Distance > FireRange;
		DrawDebugSphere(GetWorld(), GetActorLocation(), FireRange, 16, OutOfRange ? FColor::Red : FColor::Green);

		if(!OutOfRange)
		{
			FHitResult Hit;
			GetWorld()->LineTraceSingleByChannel(Hit, GetActorLocation(), Tank->GetActorLocation(), ECC_Visibility);
			ATank* Target = Cast<ATank>(Hit.GetActor());
			DrawDebugLine(GetWorld(), GetActorLocation(), Hit.ImpactPoint, Target != nullptr ? FColor::Green : FColor::Red);
			return Target != nullptr;
		}
	}

	return false;
}
