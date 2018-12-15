// Fill out your copyright notice in the Description page of Project Settings.

#include "MyCharacterAnimInstance.h"
#include "Player/MyCharacter.h"

UMyCharacterAnimInstance::UMyCharacterAnimInstance()
{
	CurrentPawnSpeed = 0.0f;
	IsInAir = false;

	static ConstructorHelpers::FObjectFinder<UAnimMontage> ATTACK_MONTAGE(TEXT("/Game/Blueprint/AM_PhaseAttack.AM_PhaseAttack"));
	if (ATTACK_MONTAGE.Succeeded()) {
		AttackMontage = ATTACK_MONTAGE.Object;
	}
	else MYLOG(Warning, TEXT("Failed to load ATTACK_MONTAGE"));
}

void UMyCharacterAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);

	auto Pawn = TryGetPawnOwner();
	if (::IsValid(Pawn)) {
		CurrentPawnSpeed = Pawn->GetVelocity().Size();
		auto Character = Cast<AMyCharacter>(Pawn);
		if (Character) {
			IsInAir = Character->GetMovementComponent()->IsFalling();
			IsInDash = Character->OnDash();
			IsDie = Character->OnDie();
			IsHitting = Character->OnHit();
		}
	}
}

void UMyCharacterAnimInstance::PlayerAttackMontage()
{
	Montage_Play(AttackMontage, 1.0f);
}

void UMyCharacterAnimInstance::JumpToAttackMontageSection(int NewSection)
{
	MYCHECK(Montage_IsPlaying(AttackMontage));
	Montage_JumpToSection(GetAttackMontageSectionName(NewSection), AttackMontage);
}

void UMyCharacterAnimInstance::AnimNotify_NextAttackCheck()
{
	OnNextAttackCheck.Broadcast();
}

void UMyCharacterAnimInstance::AnimNotify_Attack()
{
	OnAttack.Broadcast();
}

void UMyCharacterAnimInstance::AnimNotify_DashEnd()
{
	OnDashEnd.Broadcast();
}

void UMyCharacterAnimInstance::AnimNotify_HitEnd()
{
	OnHitEnd.Broadcast();
}

FName UMyCharacterAnimInstance::GetAttackMontageSectionName(int32 Section)
{
	MYCHECK(FMath::IsWithinInclusive<int32>(Section, 1, 4), NAME_None);
	return FName(*FString::Printf(TEXT("Attack%d"), Section));
}
