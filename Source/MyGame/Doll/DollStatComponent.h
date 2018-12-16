// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DollStatComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MYGAME_API UDollStatComponent : public UActorComponent
{
	GENERATED_BODY()
private:
	struct FDollData* StatData = nullptr;

public:	
	// Sets default values for this component's properties
	UDollStatComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	virtual void InitializeComponent() override;

public:	
	float GetAttackRange();
	float GetAttackRadius();
	float GetHeartShareTime();
	float GetWalkSpeed();
	float GetMaxHP();
	float GetDetectRadius();
};
