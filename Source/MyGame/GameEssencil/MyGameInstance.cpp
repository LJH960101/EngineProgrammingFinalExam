// Fill out your copyright notice in the Description page of Project Settings.

#include "MyGameInstance.h"

UMyGameInstance::UMyGameInstance()
{
	FString CharacterDataPath = TEXT("/Game/Data/MyPlayerData.MyPlayerData");
	static ConstructorHelpers::FObjectFinder<UDataTable> DT_MYCHARACTER(*CharacterDataPath);
	MYCHECK(DT_MYCHARACTER.Succeeded());
	MyCharacterTable = DT_MYCHARACTER.Object;
}

void UMyGameInstance::Init()
{
	Super::Init();
}

FMyCharacterData * UMyGameInstance::GetMyCharacterData()
{
	return MyCharacterTable->FindRow<FMyCharacterData>(TEXT("0"), TEXT(""));
}
