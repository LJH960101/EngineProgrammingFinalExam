// Fill out your copyright notice in the Description page of Project Settings.

#include "MyCharacterHPWidget.h"
#include "Components/ProgressBar.h"

void UMyCharacterHPWidget::BindCharacterStat(UMyCharacterStatComponent* NewCharacterStat)
{
	MYCHECK(nullptr != NewCharacterStat);

	StatData = NewCharacterStat;
	NewCharacterStat->OnHPChanged.AddUObject(this, &UMyCharacterHPWidget::UpdateHPWidget);
}

void UMyCharacterHPWidget::NativeConstruct()
{
	Super::NativeConstruct();
	HPProgressBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("PB_HPBAR")));
	MYCHECK(nullptr != HPProgressBar);
	UpdateHPWidget();
}

void UMyCharacterHPWidget::UpdateHPWidget()
{
	if (StatData.IsValid()) {
		if (nullptr != HPProgressBar) {
			HPProgressBar->SetPercent(StatData->GetHpRate());
		}
	}
}
