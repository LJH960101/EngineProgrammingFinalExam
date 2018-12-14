// Fill out your copyright notice in the Description page of Project Settings.

#include "BTDecorator_IsInAttackRange.h"
#include "Doll/DollAIController.h"
#include "Doll/Doll.h"
#include "Monster/Boss.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTDecorator_IsInAttackRange::UBTDecorator_IsInAttackRange()
{
	NodeName = TEXT("CanAttack");
}

bool UBTDecorator_IsInAttackRange::CalculateRawConditionValue(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory) const
{
	bool bResult = Super::CalculateRawConditionValue(OwnerComp, NodeMemory);
	auto ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (nullptr == ControllingPawn)
		return false;
	auto Target = Cast<ABoss>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(ADollAIController::TargetKey));
	if (nullptr == Target)
		return false;
	bResult = (Target->GetDistanceTo(ControllingPawn) <= 300.0f);
	return bResult;
}
