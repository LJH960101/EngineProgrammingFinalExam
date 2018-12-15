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

	// 보스의 체력으로 최대 패턴 수를 정한다.
	uint8 maxPattern = 5U;
	if (boss->HPComponent->GetHpRate() >= 0.5f) maxPattern = 8U;

	// 20%의 확률로 특별 패턴 시행
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