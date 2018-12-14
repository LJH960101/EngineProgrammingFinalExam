// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_FinishMoveToCL.h"
#include "Doll/DollAIController.h"
#include "Doll/Doll.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_FinishMoveToCL::UBTTask_FinishMoveToCL() {
	NodeName = TEXT("FinishMoveToCL");
}
EBTNodeResult::Type UBTTask_FinishMoveToCL::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	auto ControllingPawn = Cast<ADoll>(OwnerComp.GetAIOwner()->GetPawn());
	if (nullptr == ControllingPawn)
		return EBTNodeResult::Failed;
	ControllingPawn->SetControlMoveState(false);
	return EBTNodeResult::Succeeded;
}
