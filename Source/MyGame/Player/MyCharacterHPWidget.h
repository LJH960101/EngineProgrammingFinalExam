// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "MyGame.h"
#include "Blueprint/UserWidget.h"
#include "MyCharacterHPWidget.generated.h"

/**
 * 
 */
class UHPComponent;

UCLASS()
class MYGAME_API UMyCharacterHPWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void BindCharacterStat(UHPComponent* NewHpComponent);

protected:
	virtual void NativeConstruct() override;
	void UpdateHPWidget();

private:
	TWeakObjectPtr<UHPComponent> HPData;

	UPROPERTY()
	class UProgressBar* HPProgressBar;
};
