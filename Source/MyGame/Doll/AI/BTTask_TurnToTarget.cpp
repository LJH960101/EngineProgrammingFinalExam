// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_TurnToTarget.h"
#include "Doll/DollAIController.h"
#include "Doll/Doll.h"
#include "Monster/Boss.h"
#include "Monster/BossAIController.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_TurnToTarget::UBTTask_TurnToTarget()
{
	NodeName = TEXT("Turn");
}

EBTNodeResult::Type UBTTask_TurnToTarget::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	auto Doll = Cast<ADoll>(OwnerComp.GetAIOwner()->GetPawn());
	// Doll狼 贸府
	if (nullptr != Doll)
	{
		auto Target = Cast<ABoss>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(ADollAIController::TargetKey));
		FVector LookVector = Target->GetActorLocation() - Doll->GetActorLocation();
		LookVector.Z = 0.0f;
		FRotator TargetRot = FRotationMatrix::MakeFromX(LookVector).Rotator();
		Doll->SetActorRotation(TargetRot);

		return EBTNodeResult::Succeeded;
	}

	// Boss狼 贸府
	auto Boss = Cast<ABoss>(OwnerComp.GetAIOwner()->GetPawn());
	if (nullptr != Boss)
	{
		auto Target = Cast<ACharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(ABossAIController::MinTargetKey));
		FVector LookVector = Target->GetActorLocation() - Boss->GetActorLocation();
		LookVector.Z = 0.0f;
		FRotator TargetRot = FRotationMatrix::MakeFromX(LookVector).Rotator();
		Boss->SetActorRotation(TargetRot);

		return EBTNodeResult::Succeeded;		
	}

	return EBTNodeResult::Failed;


}
