// Fill out your copyright notice in the Description page of Project Settings.

#include "BossAnimInstance.h"
#include "Monster/Boss.h"

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
		}
	}
}
