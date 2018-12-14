// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "MyGame.h"
#include "GameFramework/Character.h"
#include "Monster/Monster.h"
#include "Monster/BossAnimInstance.h"
#include "Boss.generated.h"

UCLASS()
class MYGAME_API ABoss : public ACharacter, public IMonster
{
	GENERATED_BODY()

private:
	class UHPComponent* HPComponent;
	class UBossStatComponent* StatComponent;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	bool IsDie;

	UPROPERTY()
	class UBossAnimInstance* MyAnim;

public:

private:
	void Die();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Sets default values for this character's properties
	ABoss();

	// Called every frame
	virtual void Tick(float DeltaTime) override;
	bool OnDie();

	virtual void PostInitializeComponents() override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, class AActor* DamageCauser) override;
};
