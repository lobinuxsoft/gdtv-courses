// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile.h"

#include "GameFramework/ProjectileMovementComponent.h"

// Sets default values
AProjectile::AProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Projectile Mesh"));
	RootComponent = ProjectileMesh;

	MovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Movement Component"));
	MovementComponent->MaxSpeed = 1300.f;
	MovementComponent->InitialSpeed = 1300.f;
}

// Called when the game starts or when spawned
void AProjectile::BeginPlay()
{
	Super::BeginPlay();

	ProjectileMesh->OnComponentHit.AddDynamic(this, &AProjectile::OnHit);
}

// Called every frame
void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	FString HitCompName = FString::Printf(TEXT("HitComp: %s"), *HitComp->GetName());
	FString OtherActorName = FString::Printf(TEXT("OtherActor: %s"), *OtherActor->GetName());
	FString OtherCompName = FString::Printf(TEXT("OtherComp: %s"), *OtherComp->GetName());
	
	if(GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Black, HitCompName);
		GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Blue, OtherActorName);
		GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Cyan, OtherCompName);
	}
}

