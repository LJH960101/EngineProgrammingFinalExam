// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Monster/Boss.h"
#include "BossAnimInstance.generated.h"

DECLARE_MULTICAST_DELEGATE(FNotifyDelegate);

UCLASS()
class MYGAME_API UBossAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	float CurrentPawnSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	bool IsDie;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	bool OnAttack;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	EBossPattern CurrentPattern;
	UFUNCTION()
	void AnimNotify_AttackEnd();
	UFUNCTION()
	void AnimNotify_BasicAttack();
	UFUNCTION()
	void AnimNotify_GatheringStart();
	UFUNCTION()
	void AnimNotify_GatheringEnd();
	UFUNCTION()
	void AnimNotify_ScatterStart();
	UFUNCTION()
	void AnimNotify_ScatterEnd();
	UFUNCTION()
	void AnimNotify_Jumping();
	UFUNCTION()
	void AnimNotify_HeartShare();
	UFUNCTION()
	void AnimNotify_TransLocation();
	UFUNCTION()
	void AnimNotify_Tonado();

public:
	UBossAnimInstance();
	virtual void NativeUpdateAnimation(float DeltaTime) override;

	FNotifyDelegate AttackEnd, BasicAttack, GatheringStart, GatheringEnd,
		ScatterStart, ScatterEnd, Jumping, HeartShare, TransLocation, Tonado;
};
