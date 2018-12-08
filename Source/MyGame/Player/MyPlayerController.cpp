// Fill out your copyright notice in the Description page of Project Settings.

#include "MyPlayerController.h"
#include "Components/WidgetComponent.h"

AMyPlayerController::AMyPlayerController() {
	static ConstructorHelpers::FClassFinder<UUserWidget> WG_HUD(TEXT("/Game/Widget/HUDWidget.HUDWidget_C"));
	if (WG_HUD.Succeeded()) {
		wHUD = WG_HUD.Class;
	}
	else MYLOG(Warning, TEXT("Failed to find WG_HUD"));
}

// Called when the game starts or when spawned
void AMyPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (wHUD)
	{
		auto myHUD = CreateWidget<UUserWidget>(this, wHUD);

		if (myHUD) {
			myHUD->AddToViewport();
		}
	}
}