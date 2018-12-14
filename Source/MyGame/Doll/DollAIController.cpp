// Fill out your copyright notice in the Description page of Project Settings.

#include "DollAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"

const FName ADollAIController::HomePosKey(TEXT("HomePos"));
const FName ADollAIController::PatrolPosKey(TEXT("PatrolPos"));
const FName ADollAIController::TargetKey(TEXT("Target"));
const FName ADollAIController::ControlLocationKey(TEXT("ControlLocation"));
const FName ADollAIController::OnControlMoveKey(TEXT("OnControlMove"));

ADollAIController::ADollAIController()
{
	static ConstructorHelpers::FObjectFinder<UBlackboardData> BBObject(TEXT("/Game/Blueprint/Doll/BB_DOLL.BB_DOLL"));
	if (BBObject.Succeeded())
	{
		BBAsset = BBObject.Object;
	}

	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BTObject(TEXT("/Game/Blueprint/Doll/BT_DOLL.BT_DOLL"));
	if (BTObject.Succeeded())
	{
		BTAsset = BTObject.Object;
	}
}

void ADollAIController::SetControlMoveState(bool isOn)
{
	Blackboard->SetValueAsBool(OnControlMoveKey, isOn);
}

bool ADollAIController::GetControlMoveState()
{
	return 	Blackboard->GetValueAsBool(OnControlMoveKey);
}

void ADollAIController::Possess(APawn* InPawn)
{
	Super::Possess(InPawn);
	if (UseBlackboard(BBAsset, Blackboard))
	{
		Blackboard->SetValueAsVector(HomePosKey, InPawn->GetActorLocation());
		if (!RunBehaviorTree(BTAsset))
		{
			MYLOG(Error, TEXT("AIController couldn't run behavior tree!"));
		}
	}
}

void ADollAIController::SetControlLocation(const FVector & location)
{
	Blackboard->SetValueAsVector(ControlLocationKey, location);
}
