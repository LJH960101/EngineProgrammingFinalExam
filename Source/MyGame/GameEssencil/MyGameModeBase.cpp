// Fill out your copyright notice in the Description page of Project Settings.

#include "MyGameModeBase.h"
#include "Player/MyCharacter.h"
#include "Player/MyPlayerController.h"

AMyGameModeBase::AMyGameModeBase() {
	DefaultPawnClass = AMyCharacter::StaticClass();
	PlayerControllerClass = AMyPlayerController::StaticClass();
}