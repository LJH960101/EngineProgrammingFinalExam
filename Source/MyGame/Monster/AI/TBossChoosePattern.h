// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "MyGame.h"
#include "BehaviorTree/BTTaskNode.h"
#include "TBossChoosePattern.generated.h"

/**
 * 
 */
UCLASS()
class MYGAME_API UTBossChoosePattern : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UTBossChoosePattern();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
