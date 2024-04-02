// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MovingPlatform.generated.h"

UCLASS()
class OBSTACLEASSAULT_API AMovingPlatform : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AMovingPlatform();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:

  UPROPERTY(EditAnywhere, Category = "Moving")
  FVector PlatformVelocity{FVector(100, 0, 0)};

  UPROPERTY(EditAnywhere, Category = "Moving")
  float MoveDistance{100};

  UPROPERTY(EditAnywhere, Category="Rotation")
  FRotator RotationVelocity;

  FVector StartLocation;

  /**
   * Move the platform when is called.
   * @param DeltaTime time between frames.
   */
  void MovePlatform(float DeltaTime);

  /**
   * Rotate the platform when is called.
   * @param DeltaTime time between frames.
   */
  void RotatePlatform(float DeltaTime);

  FORCEINLINE bool ShouldPlatformReturn() const;
  FORCEINLINE float GetDistanceMove() const;
};
