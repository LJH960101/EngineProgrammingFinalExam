// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HPComponent.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnHPChangeDelegate);
DECLARE_MULTICAST_DELEGATE(FOnDieDelegate);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MYGAME_API UHPComponent : public UActorComponent
{
	GENERATED_BODY()
private:
	float _hp;
	float _maxHp;

public:
	FOnHPChangeDelegate OnHPChanged;
	FOnDieDelegate OnDie;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Sets default values for this component's properties
	UHPComponent();
	float TakeDamage(float damage);
	float HealHP(float healAmount);
	float GetHp();
	float GetHpRate();
	void SetMaxHP(float maxHp);
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
};
