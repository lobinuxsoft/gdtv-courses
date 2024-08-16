// Fill out your copyright notice in the Description page of Project Settings.


#include "Tank.h"

#include "EnhancedInputComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"

ATank::ATank()
{
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
	SpringArmComp->SetupAttachment(RootComponent);

	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera Component"));
	CameraComp->SetupAttachment(SpringArmComp);
}

void ATank::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ATank::Move);
		EnhancedInputComponent->BindAction(TurnAction, ETriggerEvent::Triggered, this, &ATank::Turn);
		EnhancedInputComponent->BindAction(RotateTurretAction, ETriggerEvent::Triggered, this, &ATank::TurretRotation);
	}
}

void ATank::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(PlayerControllerRef)
	{
		FHitResult HitResult;
		PlayerControllerRef->GetHitResultUnderCursor(ECC_Visibility, false, HitResult);
		DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, 10, 12, FColor::Emerald, false, -1);
	}
}

void ATank::BeginPlay()
{
	Super::BeginPlay();

	PlayerControllerRef = Cast<APlayerController>(GetController());
}

void ATank::Move(const FInputActionValue& Value)
{
	AddActorLocalOffset(FVector::ForwardVector * Value.Get<float>() * MoveSpeed * UGameplayStatics::GetWorldDeltaSeconds(this), true);
}

void ATank::Turn(const FInputActionValue& Value)
{
	FRotator DeltaRotation = FRotator::ZeroRotator;
	DeltaRotation.Yaw = Value.Get<float>() * TurnRate * UGameplayStatics::GetWorldDeltaSeconds(this);
	AddActorLocalRotation(DeltaRotation, true);
}

void ATank::TurretRotation(const FInputActionValue& Value)
{
	// if(APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	// {
	// 	FHitResult HitResult;
	// 	PlayerController->GetHitResultAtScreenPosition(Value.Get<FVector2d>() * 100, ECC_Visibility, false, HitResult);
	// 	DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, 10, 12, FColor::Red, false, -1);
	// }
}
