// Fill out your copyright notice in the Description page of Project Settings.

#include "MyCharacterHPWidget.h"
#include "Components/ProgressBar.h"
#include "Player/MyCharacterStatComponent.h"
#include "Common/HPComponent.h"

void UMyCharacterHPWidget::BindCharacterStat(UHPComponent* NewHpComponent, UMyCharacterStatComponent* NewStatComponent)
{
	MYCHECK(nullptr != NewHpComponent);
	MYCHECK(nullptr != NewStatComponent);

	HPData = NewHpComponent;
	StatData = NewStatComponent;
	NewHpComponent->OnHPChanged.AddUObject(this, &UMyCharacterHPWidget::UpdateHPWidget);
}
void UMyCharacterHPWidget::NativeTick(const FGeometry& MyGeometry, float DeltaTime)
{
	Super::NativeTick(MyGeometry, DeltaTime);
	DashProgressBar->SetPercent(StatData->GetDashRate());
}

void UMyCharacterHPWidget::NativeConstruct()
{
	Super::NativeConstruct();
	HPProgressBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("PB_HPBAR")));
	DashProgressBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("PB_DASH_BAR")));
	MYCHECK(nullptr != HPProgressBar);
	MYCHECK(nullptr != DashProgressBar);
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