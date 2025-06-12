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
	UGameplayStatics::SpawnSoundAttached(MuzzleSound, Mesh, TEXT("MuzzleFlashSocket"));
	
	FHitResult HitInfo;
	if (GunTrace(HitInfo))
	{
		DrawDebugSphere(GetWorld(), HitInfo.Location, 5, 12, FColor::Red, false, 1);
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactEffect, HitInfo.ImpactPoint, HitInfo.ImpactNormal.Rotation());
		UGameplayStatics::SpawnSoundAtLocation(GetWorld(), ImpactSound, HitInfo.ImpactPoint);

		if (HitInfo.GetActor())
		{
			FPointDamageEvent DamageEvent(Damage, HitInfo, HitInfo.ImpactNormal, nullptr);
			HitInfo.GetActor()->TakeDamage(Damage, DamageEvent, GetOwnerController(), this);
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

bool AGun::GunTrace(FHitResult& Hit) const
{
	if (AController* Controller = GetOwnerController())
	{
		FVector Location;
		FRotator Rotation;
		Controller->GetPlayerViewPoint(Location, Rotation);
		DrawDebugCamera(GetWorld(), Location, Rotation, 90, 2, FColor::Red, false, 1);

		FVector End = Location + Rotation.Vector() * MaxRange;
		DrawDebugLine(GetWorld(), Location, End, FColor::Red, false, 1);
			
		FCollisionQueryParams Params;
		Params.AddIgnoredActor(this);
		Params.AddIgnoredActor(GetOwner());

		return GetWorld()->LineTraceSingleByChannel(Hit, Location, End, ECC_GameTraceChannel1, Params);
	}

	return false;
}

AController* AGun::GetOwnerController() const
{
	if (APawn* OwnerPawn = Cast<APawn>(GetOwner()))
		return OwnerPawn->GetController();

	return nullptr;
}
