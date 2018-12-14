// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "MyGame.h"
#include "Components/ActorComponent.h"
#include "MyCharacterStatComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MYGAME_API UMyCharacterStatComponent : public UActorComponent
{
	GENERATED_BODY()
private:
	struct FMyCharacterData* StatData = nullptr;
	float _dashGauage;

public:	
	// Sets default values for this component's properties
	UMyCharacterStatComponent();
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	float GetAttackDamage();
	float GetDashDamage();
	float GetBulletSpeed();
	float GetDashRange();
	float GetJumpPower();
	float GetDashRate();
	void SetDashGauage(float dashGauae);
	float GetDashRadiusRange();
	float GetMaxHp();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	virtual void InitializeComponent() override;
		
};
