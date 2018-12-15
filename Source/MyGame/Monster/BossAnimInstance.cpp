// Fill out your copyright notice in the Description page of Project Settings.

#include "BossAnimInstance.h"
#include "Monster/Boss.h"
#include "DrawDebugHelpers.h"

void UBossAnimInstance::AnimNotify_AttackEnd()
{
	auto Pawn = TryGetPawnOwner();
	if (::IsValid(Pawn)) {
		auto Boss = Cast<ABoss>(Pawn);
		MYCHECK(Boss != nullptr);
		Boss->SetOnAttack(false);
	}
}

UBossAnimInstance::UBossAnimInstance()
{
	CurrentPawnSpeed = 0.0f;
	IsDie = false;
}

void UBossAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);

	auto Pawn = TryGetPawnOwner();
	if (::IsValid(Pawn)) {
		CurrentPawnSpeed = Pawn->GetVelocity().Size();
		auto Boss = Cast<ABoss>(Pawn);
		if (Boss) {
			IsDie = Boss->OnDie();
			OnAttack = Boss->OnAttack();
			CurrentPattern = Boss->GetCurrentPattern();
		}
	}
}

void UBossAnimInstance::AnimNotify_BasicAttack()
{
	BasicAttack.Broadcast();
}

void UBossAnimInstance::AnimNotify_GatheringStart()
{
	GatheringStart.Broadcast();
}

void UBossAnimInstance::AnimNotify_GatheringEnd()
{
	GatheringEnd.Broadcast();
}

void UBossAnimInstance::AnimNotify_ScatterStart()
{
	ScatterStart.Broadcast();
}

void UBossAnimInstance::AnimNotify_ScatterEnd()
{
	ScatterEnd.Broadcast();
}

void UBossAnimInstance::AnimNotify_Jumping()
{
	Jumping.Broadcast();
}

void UBossAnimInstance::AnimNotify_HeartShare()
{
	HeartShare.Broadcast();
}

void UBossAnimInstance::AnimNotify_TransLocation()
{
	TransLocation.Broadcast();
}

void UBossAnimInstance::AnimNotify_Tonado()
{
	Tonado.Broadcast();
}