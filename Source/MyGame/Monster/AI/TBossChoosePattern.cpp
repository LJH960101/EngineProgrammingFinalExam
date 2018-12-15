// Fill out your copyright notice in the Description page of Project Settings.

#include "TBossChoosePattern.h"
#include "Monster/BossAIController.h"
#include "Monster/Boss.h"
#include "Common/HPComponent.h"
#include "BehaviorTree/BlackboardComponent.h"


UTBossChoosePattern::UTBossChoosePattern()
{
	NodeName = TEXT("ChoosePattern");
}
EBTNodeResult::Type UTBossChoosePattern::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	ABoss* boss = Cast<ABoss>(OwnerComp.GetAIOwner()->GetPawn());
	if(boss == nullptr) return EBTNodeResult::Failed;

	// �ִ� ü���� 50% ���Ϸ� �پ����°�?
	if (!(OwnerComp.GetBlackboardComponent()->GetValueAsBool(ABossAIController::OnHalfHpKey)) && boss->HPComponent->GetHpRate() <= 0.5f) {
		// �׷��ٸ� ��� ����̵� �������� ��ȯ�Ѵ�.
		OwnerComp.GetBlackboardComponent()->SetValueAsBool(ABossAIController::OnHalfHpKey, true);
		boss->SetPattern(EBossPattern::BP_Tonado);
		return EBTNodeResult::Succeeded;
	}

	// ������ ü������ �ִ� ���� ���� ���Ѵ�.
	uint8 maxPattern = 5U;
	if (boss->HPComponent->GetHpRate() <= 0.5f) maxPattern = 7U;

	// 20%�� Ȯ���� Ư�� ���� ����
	if (FMath::RandRange(0, 5) == 1)
	{
		boss->SetPattern((EBossPattern)FMath::RandRange(1, maxPattern));
	}
	else {
		if (FMath::RandRange(0, 2)==1) boss->SetPattern(EBossPattern::BP_BasicAttack1);
		else boss->SetPattern(EBossPattern::BP_BasicAttack2);
	}

	return EBTNodeResult::Succeeded;
}