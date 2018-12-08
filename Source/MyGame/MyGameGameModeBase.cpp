// Fill out your copyright notice in the Description page of Project Settings.

#include "MyGameGameModeBase.h"
#include "Player/MyCharacter.h"
#include "Player/MyPlayerController.h"

AMyGameGameModeBase::AMyGameGameModeBase() {
	DefaultPawnClass = AMyCharacter::StaticClass();
	PlayerControllerClass = AMyPlayerController::StaticClass();
}