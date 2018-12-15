// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "MyGame.h"
#include "AIController.h"
#include "BossAIController.generated.h"

enum class EBossPattern : uint8;

/**
 * 
 */
UCLASS()
class MYGAME_API ABossAIController : public AAIController
{
	GENERATED_BODY()
public:
	static const FName MinTargetKey;
	static const FName MaxTargetKey;
	static const FName CurrentStateKey;
	static const FName OnAttackKey;
	
public:
	ABossAIController();
	virtual void Possess(APawn* InPawn) override;
	void ChangeOnAttack(bool isOn);
	void SetCurrentState(EBossPattern pattern);

private:
	UPROPERTY()
	class UBehaviorTree* BTAsset;

	UPROPERTY()
	class UBlackboardData* BBAsset;
};
