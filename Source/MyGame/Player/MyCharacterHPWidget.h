// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "MyGame.h"
#include "Blueprint/UserWidget.h"
#include "Player/MyCharacterStatComponent.h"
#include "MyCharacterHPWidget.generated.h"

/**
 * 
 */
UCLASS()
class MYGAME_API UMyCharacterHPWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void BindCharacterStat(UMyCharacterStatComponent* NewCharacterStat);

protected:
	virtual void NativeConstruct() override;
	void UpdateHPWidget();

private:
	TWeakObjectPtr<UMyCharacterStatComponent> StatData;

	UPROPERTY()
	class UProgressBar* HPProgressBar;
};
