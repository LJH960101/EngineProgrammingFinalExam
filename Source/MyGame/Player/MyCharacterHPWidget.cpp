// Fill out your copyright notice in the Description page of Project Settings.

#include "MyCharacterHPWidget.h"
#include "Components/ProgressBar.h"
#include "Common/HPComponent.h"

void UMyCharacterHPWidget::BindCharacterStat(UHPComponent* NewHpComponent)
{
	MYCHECK(nullptr != NewHpComponent);

	HPData = NewHpComponent;
	NewHpComponent->OnHPChanged.AddUObject(this, &UMyCharacterHPWidget::UpdateHPWidget);
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
	if (HPData.IsValid()) {
		if (nullptr != HPProgressBar) {
			HPProgressBar->SetPercent(HPData->GetHpRate());
		}
	}
}
