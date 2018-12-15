// Fill out your copyright notice in the Description page of Project Settings.

#include "TBoss_ChangeState.h"
#include "Monster/BossAIController.h"
#include "Monster/Boss.h"
#include "Common/HPComponent.h"
#include "BehaviorTree/BlackboardComponent.h"


UTBoss_ChangeState::UTBoss_ChangeState()
{
	NodeName = TEXT("ChangeState");
}

EBTNodeResult::Type UTBoss_ChangeState::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	ABoss* boss = Cast<ABoss>(OwnerComp.GetAIOwner()->GetPawn());
	if (boss == nullptr) return EBTNodeResult::Failed;
	boss->SetPattern(EBossPattern::BP_NotAttacking);
	boss->SetOnAttack(false);
	return EBTNodeResult::Succeeded;
}