// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterCharacter.h"
#include "EnhancedInputComponent.h"
#include "Gun.h"

// Sets default values
AShooterCharacter::AShooterCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AShooterCharacter::BeginPlay()
{
	Super::BeginPlay();

	Health = MaxHealth;
	
	Gun = GetWorld()->SpawnActor<AGun>(GunClass);
	GetMesh()->HideBoneByName(TEXT("weapon_r"), PBO_None);
	Gun->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("WeaponSocket"));
	Gun->SetOwner(this);
}

bool AShooterCharacter::IsDead() const { return Health <= 0; }

// Called every frame
void AShooterCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AShooterCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AShooterCharacter::Move);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AShooterCharacter::Look);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &AShooterCharacter::Jump);
		EnhancedInputComponent->BindAction(ShootAction, ETriggerEvent::Started, this, &AShooterCharacter::Shoot);
	}
}

float AShooterCharacter::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser)
{
	float DamageToApply = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	DamageToApply = FMath::Min(Health, DamageToApply);
	Health -= DamageToApply;

	if (GEngine)
		GEngine->AddOnScreenDebugMessage(0, 5, FColor::Red, FString::Printf(TEXT("%s Health: %f"), *GetActorNameOrLabel(),Health));

	return DamageToApply;
}

void AShooterCharacter::Move(const FInputActionValue& Value)
{
	FVector2D InputValue = Value.Get<FVector2D>();
	AddMovementInput(GetActorForwardVector() * InputValue.Y + GetActorRightVector() * InputValue.X);
}

void AShooterCharacter::Look(const FInputActionValue& Value)
{
	FVector2D InputValue = Value.Get<FVector2D>();
	AddControllerPitchInput(-InputValue.Y * RotationRate * GetWorld()->GetDeltaSeconds());
	AddControllerYawInput(InputValue.X * RotationRate * GetWorld()->GetDeltaSeconds());
}

void AShooterCharacter::Shoot(const FInputActionValue& Value)
{
	if (Gun)
		Gun->PullTrigger();
}
