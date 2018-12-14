// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "MyGame.h"
#include "Engine/DataTable.h"
#include "Engine/GameInstance.h"
#include "MyGameInstance.generated.h"

USTRUCT(BlueprintType)
struct FMyCharacterData : public FTableRowBase
{
	GENERATED_BODY()
	
public:
	FMyCharacterData() : PlayerHP(10.0f), PlayerAttackDamage(5.0f), PlayerDashDamage(10.0f), PlayerBulletSpeed(100.0f), PlayerDashRange(1000.0f), PlayerJumpPower(800.0f), PlayerDashRadiusRange(200.0f), PlayerDashCoolTime(5.0f){}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	float PlayerHP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	float PlayerAttackDamage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	float PlayerDashDamage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	float PlayerBulletSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	float PlayerDashRange;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	float PlayerJumpPower;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	float PlayerDashRadiusRange;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	float PlayerDashCoolTime;
};

UCLASS()
class MYGAME_API UMyGameInstance : public UGameInstance
{
	GENERATED_BODY()
private:
	UPROPERTY()
	UDataTable* MyCharacterTable;

public:
	UMyGameInstance();
	virtual void Init() override;
	FMyCharacterData* GetMyCharacterData();
};
