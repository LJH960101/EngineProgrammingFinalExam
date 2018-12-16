// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "MyGame.h"
#include "GameFramework/Character.h"
#include "Monster/Monster.h"
#include "Boss.generated.h"

UENUM(BlueprintType)		//"BlueprintType" is essential to include
enum class EBossPattern : uint8
{
	BP_NotAttacking	UMETA(DisplayName = "Not Attacking"),
	BP_BasicAttack1 	UMETA(DisplayName = "Basic Attack1"),
	BP_BasicAttack2 	UMETA(DisplayName = "Basic Attack2"),
	BP_Gathering 	UMETA(DisplayName = "Gathering"),
	BP_Scatter 	UMETA(DisplayName = "Scatter"),
	BP_Jumping 	UMETA(DisplayName = "Jumping"),
	BP_HeartShare 	UMETA(DisplayName = "Heart Share"),
	BP_TransLocation 	UMETA(DisplayName = "TransLocation"),
	BP_Tonado 	UMETA(DisplayName = "Tonado"),
	BP_MAX 	UMETA(DisplayName = "MAX")
};

DECLARE_MULTICAST_DELEGATE(FOnBossHpChanged);

UCLASS()
class MYGAME_API ABoss : public ACharacter, public IMonster
{
	GENERATED_BODY()
	
public:
	UPROPERTY(VisibleAnywhere, Category = Stat)
	class UHPComponent* HPComponent;

	UPROPERTY(VisibleAnywhere, Category = Stat)
	class UBossStatComponent* StatComponent;

private:
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	EBossPattern CurrentPattern;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	bool IsDie;

	UPROPERTY()
	class UBossAnimInstance* MyAnim;
	bool bOnAttack;

	// Pattern Particles
	UPROPERTY()
	UParticleSystem* JumpingParticle;
	UPROPERTY()
	UParticleSystem* GatheringParticle;
	UPROPERTY()
	UParticleSystem* GatheringFailParticle;
	UPROPERTY()
	UParticleSystem* ScatterParticle;
	UPROPERTY()
	UParticleSystem* ScatterImpactParticle;
	UPROPERTY()
	UParticleSystem* HeartShareParticle;
private:
	void Die();
	void BindAnimationEvents();
	void BasicAttack();
	void Jumping();
	void GatheringStart();
	void GatheringEnd();
	void ScatterStart();
	void ScatterEnd();
	void TransLocation();
	void Tonado();
	void HeartShare();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	FOnBossHpChanged OnBossHpChanged;

	// Sets default values for this character's properties
	ABoss();

	// Called every frame
	virtual void Tick(float DeltaTime) override;
	bool OnDie();

	bool OnAttack();
	void SetOnAttack(bool isOn);

	void SetPattern(EBossPattern pattern);
	EBossPattern GetCurrentPattern();

	virtual void PostInitializeComponents() override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, class AActor* DamageCauser) override;
};
