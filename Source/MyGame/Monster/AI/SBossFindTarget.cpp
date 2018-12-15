// Fill out your copyright notice in the Description page of Project Settings.

#include "SBossFindTarget.h"
#include "Doll/Doll.h"
#include "Player/MyCharacter.h"
#include "Monster/Monster.h"
#include "Monster/Boss.h"
#include "Monster/BossAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "DrawDebugHelpers.h"

USBossFindTarget::USBossFindTarget()
{
	NodeName = TEXT("FindTarget");
	Interval = 1.0f;
}

void USBossFindTarget::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (nullptr == ControllingPawn) return;

	UWorld* World = ControllingPawn->GetWorld();
	FVector Center = ControllingPawn->GetActorLocation();
	float DetectRadius = 2500.0f;

	if (nullptr == World) return;
	TArray<FOverlapResult> OverlapResults;
	FCollisionQueryParams CollisionQueryParam(NAME_None, false, ControllingPawn);
	bool bResult = World->OverlapMultiByChannel(
		OverlapResults,
		Center,
		FQuat::Identity,
		ECollisionChannel::ECC_Pawn,
		FCollisionShape::MakeSphere(DetectRadius),
		CollisionQueryParam
	);
	if (bResult)
	{
		float minDis = 9999999.f, maxDis = -99999.f;
		AActor* minAct = OverlapResults[0].GetActor();
		AActor* maxAct = OverlapResults[0].GetActor();
		for (auto OverlapResult : OverlapResults)
		{
			if (Cast<IPC>(OverlapResult.GetActor()) == nullptr) continue;
			float dist = FVector::Dist(OwnerComp.GetOwner()->GetActorLocation(),
				OverlapResult.GetActor()->GetActorLocation());
			if (dist > maxDis) {
				maxDis = dist;
				maxAct = OverlapResult.GetActor();
			}
			if (dist < minDis) {
				minDis = dist;
				minAct = OverlapResult.GetActor();
			}
		}
		OwnerComp.GetBlackboardComponent()->SetValueAsObject(ABossAIController::MinTargetKey, Cast<ACharacter>(minAct));
		OwnerComp.GetBlackboardComponent()->SetValueAsObject(ABossAIController::MaxTargetKey, Cast<ACharacter>(maxAct));
	}
	else
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsObject(ABossAIController::MinTargetKey, nullptr);
		OwnerComp.GetBlackboardComponent()->SetValueAsObject(ABossAIController::MaxTargetKey, nullptr);
	}

	DrawDebugSphere(World, Center, DetectRadius, 16, FColor::Red, false, 0.2f);
}