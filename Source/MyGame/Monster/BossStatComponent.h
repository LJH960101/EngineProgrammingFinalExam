// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "MyGame.h"
#include "Components/ActorComponent.h"
#include "BossStatComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MYGAME_API UBossStatComponent : public UActorComponent
{
	GENERATED_BODY()
	
private:
	struct FBossData* StatData = nullptr;
			   
public:	
	// Sets default values for this component's properties
	UBossStatComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	virtual void InitializeComponent() override;

public:	
	float GetMaxHp();
	float GetDetectRadius();
	float GetPatternRate();
	float GetWalkSpeed();
	float GetBasicAttackHeight();
	float GetBasicAttackRadius();
	float GetBasicAttackDamage();
	float GetJumpingDamage();
	float GetGatheringStartHeal();
	float GetGatheringEndHeal();
	float GetGatheringDist();
	float GetGatheringDamage();
	float GetScatterDist();
	float GetScatterDamage();
};