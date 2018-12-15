// Fill out your copyright notice in the Description page of Project Settings.

#include "BossAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "Monster/Boss.h"
#include "BehaviorTree/BlackboardComponent.h"

const FName ABossAIController::MinTargetKey(TEXT("MinTarget"));
const FName ABossAIController::MaxTargetKey(TEXT("MaxTarget"));
const FName ABossAIController::CurrentStateKey(TEXT("CurrentState"));
const FName ABossAIController::OnAttackKey(TEXT("OnAttack"));
const FName ABossAIController::OnHalfHpKey(TEXT("OnHalfHp"));

ABossAIController::ABossAIController()
{
	static ConstructorHelpers::FObjectFinder<UBlackboardData> BBObject(TEXT("/Game/Blueprint/Boris/BB_Boss.BB_Boss"));
	if (BBObject.Succeeded())
	{
		BBAsset = BBObject.Object;
	}

	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BTObject(TEXT("/Game/Blueprint/Boris/BT_Boss.BT_Boss"));
	if (BTObject.Succeeded())
	{
		BTAsset = BTObject.Object;
	}
}

void ABossAIController::SetCurrentState(EBossPattern pattern)
{
	Blackboard->SetValueAsEnum(CurrentStateKey, (uint8)pattern);
}

void ABossAIController::ChangeOnAttack(bool isOn)
{
	Blackboard->SetValueAsBool(OnAttackKey, isOn);
}

void ABossAIController::TransTarget()
{
	auto minTarget = Blackboard->GetValueAsObject(MinTargetKey);
	Blackboard->SetValueAsObject(MinTargetKey, Blackboard->GetValueAsObject(MaxTargetKey));
	Blackboard->SetValueAsObject(MaxTargetKey, minTarget);
}

void ABossAIController::Possess(APawn* InPawn)
{
	Super::Possess(InPawn);
	if (UseBlackboard(BBAsset, Blackboard))
	{
		if (!RunBehaviorTree(BTAsset))
		{
			MYLOG(Error, TEXT("AIController couldn't run behavior tree!"));
		}
	}
}