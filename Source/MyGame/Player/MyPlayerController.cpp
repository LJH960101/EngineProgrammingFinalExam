// Fill out your copyright notice in the Description page of Project Settings.

#include "MyPlayerController.h"
#include "Components/WidgetComponent.h"
#include "Components/ProgressBar.h"
#include "Common/HPComponent.h"
#include "Monster/Boss.h"

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
		myHUD = CreateWidget<UUserWidget>(this, wHUD);

		if (myHUD) {
			myHUD->AddToViewport();
		}
	}

	TArray<AActor*> bosses;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABoss::StaticClass(), bosses);
	MYCHECK(bosses.Num() != 0);
	boss = Cast<ABoss>(bosses[0]);
	boss->OnBossHpChanged.AddUObject(this, &AMyPlayerController::OnChangeBossHp);
}

void AMyPlayerController::OnChangeBossHp()
{
	auto BossHPProgressBar = Cast<UProgressBar>(myHUD->GetWidgetFromName(TEXT("HP_ProgressBar")));
	BossHPProgressBar->SetPercent(boss->HPComponent->GetHpRate());
}