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

	TurretDir = GetActorForwardVector();
}

void ATank::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ATank::Move);
		EnhancedInputComponent->BindAction(TurnAction, ETriggerEvent::Triggered, this, &ATank::Turn);
		EnhancedInputComponent->BindAction(RotateTurretAction, ETriggerEvent::Triggered, this, &ATank::TurretRotation);
		EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Started, this, &ATank::Fire);
	}
}

void ATank::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// if(PlayerControllerRef)
	// {
	// 	FHitResult HitResult;
	// 	PlayerControllerRef->GetHitResultUnderCursor(ECC_Visibility, false, HitResult);
	// 	DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, 10, 12, FColor::Emerald, false, -1);
	//
	// 	RotateTurret(HitResult.ImpactPoint);
	// }
}

void ATank::HandleDestruction()
{
	Super::HandleDestruction();
	SetActorHiddenInGame(true);
	SetActorEnableCollision(false);
	SetActorTickEnabled(false);
}

void ATank::BeginPlay()
{
	Super::BeginPlay();

	TankPlayerController = Cast<APlayerController>(GetController());
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
	FVector2D InputValue = Value.Get<FVector2d>();

	FVector Forward = FVector::VectorPlaneProject(CameraComp->GetForwardVector(), FVector::UpVector);
	FVector Right = FVector::VectorPlaneProject(CameraComp->GetRightVector(), FVector::UpVector);
	FVector TempDir = Forward * InputValue.Y + Right * InputValue.X;

	if(TempDir.SquaredLength() > 0)
		TurretDir = TempDir.GetSafeNormal();

	FVector2D ScreenPos = FVector2D::Zero();
	UGameplayStatics::ProjectWorldToScreen(TankPlayerController,GetActorLocation() + TurretDir * 250, ScreenPos);

	FHitResult HitResult;
	TankPlayerController->GetHitResultAtScreenPosition(ScreenPos, ECC_Visibility, false, HitResult);
	
	RotateTurret(HitResult.ImpactPoint);
}