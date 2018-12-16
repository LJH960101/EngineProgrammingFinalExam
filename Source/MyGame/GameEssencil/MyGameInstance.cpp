// Fill out your copyright notice in the Description page of Project Settings.

#include "MyGameInstance.h"

UMyGameInstance::UMyGameInstance()
{
	static ConstructorHelpers::FObjectFinder<UDataTable> DT_MYCHARACTER(TEXT("/Game/Data/MyPlayerData.MyPlayerData"));
	MYCHECK(DT_MYCHARACTER.Succeeded());
	MyCharacterTable = DT_MYCHARACTER.Object;

	static ConstructorHelpers::FObjectFinder<UDataTable> DT_BOSS(TEXT("/Game/Data/BossData.BossData"));
	MYCHECK(DT_BOSS.Succeeded());
	BossTable = DT_BOSS.Object;

	static ConstructorHelpers::FObjectFinder<UDataTable> DT_TONADO(TEXT("/Game/Data/TonadoData.TonadoData"));
	MYCHECK(DT_TONADO.Succeeded());
	TonadoTable = DT_TONADO.Object;

	static ConstructorHelpers::FObjectFinder<UDataTable> DT_DOLL(TEXT("/Game/Data/DollData.DollData"));
	MYCHECK(DT_DOLL.Succeeded());
	DollTable = DT_DOLL.Object;
}

void UMyGameInstance::Init()
{
	Super::Init();
}

FMyCharacterData * UMyGameInstance::GetMyCharacterData()
{
	return MyCharacterTable->FindRow<FMyCharacterData>(TEXT("0"), TEXT(""));
}

FTonadoData* UMyGameInstance::GetTonadoData()
{
	return TonadoTable->FindRow<FTonadoData>(TEXT("0"), TEXT(""));
}

FBossData* UMyGameInstance::GetBossData()
{
	return BossTable->FindRow<FBossData>(TEXT("0"), TEXT(""));
}

FDollData* UMyGameInstance::GetDollData()
{
	return DollTable->FindRow<FDollData>(TEXT("0"), TEXT(""));
}