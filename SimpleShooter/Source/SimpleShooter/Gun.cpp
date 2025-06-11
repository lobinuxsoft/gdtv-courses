// Fill out your copyright notice in the Description page of Project Settings.

#include "Gun.h"

#include "Engine/DamageEvents.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AGun::AGun()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(Root);
}

void AGun::PullTrigger()
{
	UGameplayStatics::SpawnEmitterAttached(MuzzleFlash, Mesh, TEXT("MuzzleFlashSocket"));

	if (APawn* OwnerPawn = Cast<APawn>(GetOwner()))
	{
		if (OwnerPawn->GetController())
		{
			FVector Location;
			FRotator Rotation;
			OwnerPawn->GetController()->GetPlayerViewPoint(Location, Rotation);
			DrawDebugCamera(GetWorld(), Location, Rotation, 90, 2, FColor::Red, false, 1);

			FVector End = Location + Rotation.Vector() * MaxRange;
			DrawDebugLine(GetWorld(), Location, End, FColor::Red, false, 1);

			FHitResult HitInfo;
			FCollisionQueryParams Params;
			Params.AddIgnoredActor(this);
			Params.AddIgnoredActor(GetOwner());

			if (GetWorld()->LineTraceSingleByChannel(HitInfo, Location, End, ECC_GameTraceChannel1, Params))
			{
				DrawDebugSphere(GetWorld(), HitInfo.Location, 5, 12, FColor::Red, false, 1);
				UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactEffect, HitInfo.ImpactPoint, HitInfo.ImpactNormal.Rotation());

				if (HitInfo.GetActor())
				{
					FPointDamageEvent DamageEvent(Damage, HitInfo, HitInfo.ImpactNormal, nullptr);
					HitInfo.GetActor()->TakeDamage(Damage, DamageEvent, OwnerPawn->GetController(), this);
				}
			}
		}
	}
}

// Called when the game starts or when spawned
void AGun::BeginPlay()
{
	Super::BeginPlay();

	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Cyan, FString::Printf(TEXT("%s"), *GetActorNameOrLabel()));
}

// Called every frame
void AGun::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
