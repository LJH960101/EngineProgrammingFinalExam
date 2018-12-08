// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "MyGame.h"
#include "GameFramework/PlayerController.h"
#include "MyPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class MYGAME_API AMyPlayerController : public APlayerController
{
	GENERATED_BODY()

private:
	UPROPERTY()
	TSubclassOf<class UUserWidget> wHUD;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	AMyPlayerController();
};
