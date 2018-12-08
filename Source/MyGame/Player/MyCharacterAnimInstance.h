// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "MyCharacterAnimInstance.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnNextAttackCheckDelegate);
DECLARE_MULTICAST_DELEGATE(FOnAttackDelegate);
DECLARE_MULTICAST_DELEGATE(FOnDashEndDelegate);

/**
 * 
 */
UCLASS()
class MYGAME_API UMyCharacterAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

	//Field
private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Pawn, Meta=(AllowPrivateAccess=true))
	float CurrentPawnSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	bool IsInAir;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	bool IsInDash;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	bool IsDie;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category=Attack, Meta=(AllowPrivateAccess = true))
	UAnimMontage* AttackMontage;

public:
	FOnNextAttackCheckDelegate OnNextAttackCheck;
	FOnAttackDelegate OnAttack;
	FOnDashEndDelegate OnDashEnd;

	//Method
public:
	UMyCharacterAnimInstance();
	virtual void NativeUpdateAnimation(float DeltaTime) override;
	void PlayerAttackMontage();
	void JumpToAttackMontageSection(int NewSection);

private:
	UFUNCTION()
	void AnimNotify_NextAttackCheck();

	UFUNCTION()
	void AnimNotify_Attack();

	UFUNCTION()
	void AnimNotify_DashEnd();

	FName GetAttackMontageSectionName(int32 Section);
};