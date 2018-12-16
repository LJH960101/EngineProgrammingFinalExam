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
	FMyCharacterData() : PlayerHP(10.0f), PlayerAttackDamage(5.0f), PlayerDashDamage(10.0f), PlayerBulletSpeed(100.0f), PlayerDashRange(1000.0f), PlayerJumpPower(800.0f), PlayerDashRadiusRange(200.0f), PlayerDashCoolTime(5.0f) {}

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

USTRUCT(BlueprintType)
struct FTonadoData : public FTableRowBase
{
	GENERATED_BODY()

	public:
	FTonadoData() : Damage(10.0f), AttackTime(2.0f), Radius(150.0f), Speed(200.0f) {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	float Damage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	float AttackTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	float Radius;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	float Speed;
};

USTRUCT(BlueprintType)
struct FDollData : public FTableRowBase
{
	GENERATED_BODY()

	public:
	FDollData() : AttackRange(200.0f), AttackRadius(50.0f), HeartShareTime(5.0f), WalkSpeed(600.0f), HP(999999.0f), DetectRadius(600.0f) {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	float AttackRange;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	float AttackRadius;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	float HeartShareTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	float WalkSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	float HP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	float DetectRadius;
};

USTRUCT(BlueprintType)
struct FBossData : public FTableRowBase
{
	GENERATED_BODY()

	public:
	FBossData() : HP(300.0f), DetectRadius(2500.0f), PatternRate(3.0f), WalkSpeed(900.0f), 
		BasicAttackHeight(300.0f), BasicAttackRadius(300.0f), BasicAttackDamage(5.f), JumpingDamage(10.0f),
		GatheringStartHeal(10.0f), GatheringEndHeal(50.0f), GatheringDist(500.0f), GatheringDamage(20.0f),
		ScatterDist(800.0f), ScatterDamage(20.0f){}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	float HP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	float DetectRadius;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	float PatternRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	float WalkSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	float BasicAttackHeight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	float BasicAttackRadius;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	float BasicAttackDamage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	float JumpingDamage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	float GatheringStartHeal;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	float GatheringEndHeal;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	float GatheringDist;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	float GatheringDamage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	float ScatterDist;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	float ScatterDamage;
};

UCLASS()
class MYGAME_API UMyGameInstance : public UGameInstance
{
	GENERATED_BODY()
private:
	UPROPERTY()
	UDataTable* MyCharacterTable;
	UDataTable* TonadoTable;
	UDataTable* BossTable;
	UDataTable* DollTable;

public:
	UMyGameInstance();
	virtual void Init() override;
	FMyCharacterData* GetMyCharacterData();
	FTonadoData* GetTonadoData();
	FBossData* GetBossData();
	FDollData* GetDollData();
};
