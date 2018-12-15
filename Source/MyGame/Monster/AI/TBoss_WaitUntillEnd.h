// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "MyGame.h"
#include "BehaviorTree/BTTaskNode.h"
#include "TBoss_WaitUntillEnd.generated.h"

/**
 * 
 */
UCLASS()
class MYGAME_API UTBoss_WaitUntillEnd : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UTBoss_WaitUntillEnd();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected:
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSecondes) override;
};
