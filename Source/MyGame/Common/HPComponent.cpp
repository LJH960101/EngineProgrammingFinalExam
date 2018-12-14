// Fill out your copyright notice in the Description page of Project Settings.

#include "HPComponent.h"

// Sets default values for this component's properties
UHPComponent::UHPComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	_hp = 100.0f;
}

float UHPComponent::TakeDamage(float damage)
{
	_hp = FMath::Clamp(_hp - damage, 0.0f, _maxHp);
	OnHPChanged.Broadcast();
	if (_hp <= KINDA_SMALL_NUMBER) OnDie.Broadcast();
	return _hp;
}

float UHPComponent::HealHP(float healAmount)
{
	_hp = FMath::Clamp(_hp + healAmount, 0.0f, _maxHp);
	OnHPChanged.Broadcast();
	return _hp;
}

float UHPComponent::GetHp()
{
	return _hp;
}

float UHPComponent::GetHpRate()
{
	return _hp / _maxHp;
}

void UHPComponent::SetMaxHP(float maxHp)
{
	_maxHp = maxHp;
	_hp = _maxHp;
	OnHPChanged.Broadcast();
}

// Called when the game starts
void UHPComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

// Called every frame
void UHPComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

