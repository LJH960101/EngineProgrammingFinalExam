// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "TBoss_ChangeState.generated.h"

/**
 * 
 */
UCLASS()
class MYGAME_API UTBoss_ChangeState : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UTBoss_ChangeState();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
