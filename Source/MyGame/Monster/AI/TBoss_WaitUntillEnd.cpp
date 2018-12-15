// Fill out your copyright notice in the Description page of Project Settings.

#include "TBoss_WaitUntillEnd.h"
#include "Monster/Boss.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

UTBoss_WaitUntillEnd::UTBoss_WaitUntillEnd()
{
	NodeName = TEXT("WaitUntilEnd");
	bNotifyTick = true;
}

EBTNodeResult::Type UTBoss_WaitUntillEnd::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);
	return EBTNodeResult::InProgress;
}

void UTBoss_WaitUntillEnd::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSecondes)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSecondes);
	auto boss = Cast<ABoss>(OwnerComp.GetAIOwner()->GetPawn());
	MYCHECK(boss != nullptr);
	if (boss->OnAttack() == false) {
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}