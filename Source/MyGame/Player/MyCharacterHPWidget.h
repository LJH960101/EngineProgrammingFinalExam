// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "MyGame.h"
#include "Blueprint/UserWidget.h"
#include "MyCharacterHPWidget.generated.h"

/**
 * 
 */
class UHPComponent;
class UMyCharacterStatComponent;

UCLASS()
class MYGAME_API UMyCharacterHPWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void BindCharacterStat(UHPComponent* NewHpComponent, UMyCharacterStatComponent* NewStatComponent);

protected:
	virtual void NativeConstruct() override;
	void UpdateHPWidget();

private:
	TWeakObjectPtr<UHPComponent> HPData;
	TWeakObjectPtr<UMyCharacterStatComponent> StatData;

	UPROPERTY()
	class UProgressBar* HPProgressBar;

	UPROPERTY()
	class UProgressBar* DashProgressBar;

	virtual void NativeTick(const FGeometry& MyGeometry, float DeltaTime) override;
};
