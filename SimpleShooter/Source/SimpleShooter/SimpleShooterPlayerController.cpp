// Fill out your copyright notice in the Description page of Project Settings.


#include "SimpleShooterPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "Blueprint/UserWidget.h"
#include "Engine/LocalPlayer.h"

void ASimpleShooterPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(
		GetLocalPlayer()))
		Subsystem->AddMappingContext(InputMappingContext, 0);

	if ((HUD = CreateWidget(this, HUDClass)))
		HUD->AddToViewport();
}

void ASimpleShooterPlayerController::GameHasEnded(AActor* EndGameFocus, bool bIsWinner)
{
	Super::GameHasEnded(EndGameFocus, bIsWinner);

	HUD->RemoveFromParent();

	if (bIsWinner)
	{
		if (UUserWidget* WinScreen = CreateWidget(this, WinScreenClass))
			WinScreen->AddToViewport();
	}
	else
	{
		if (UUserWidget* LoseScreen = CreateWidget(this, LoseScreenClass))
			LoseScreen->AddToViewport();
	}

	GetWorldTimerManager().SetTimer(RestartTimer, this, &APlayerController::RestartLevel, RestartDelay);
}
