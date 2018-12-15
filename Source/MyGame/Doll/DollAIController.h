// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "MyGame.h"
#include "AIController.h"
#include "DollAIController.generated.h"

/**
 * 
 */
UCLASS()
class MYGAME_API ADollAIController : public AAIController
{
	GENERATED_BODY()
	
public:
	ADollAIController();
	virtual void Possess(APawn* InPawn) override;
	void SetControlLocation(const FVector & location);
	void SetControlMoveState(bool isOn);
	void SetTargetObjectToNull();
	bool GetControlMoveState();

	static const FName HomePosKey;
	static const FName PatrolPosKey;
	static const FName TargetKey;
	static const FName ControlLocationKey;
	static const FName OnControlMoveKey;

private:
	UPROPERTY()
	class UBehaviorTree* BTAsset;

	UPROPERTY()
	class UBlackboardData* BBAsset;
};
