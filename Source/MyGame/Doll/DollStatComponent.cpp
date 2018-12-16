// Fill out your copyright notice in the Description page of Project Settings.

#include "DollStatComponent.h"
#include "GameEssencil/MyGameInstance.h"

// Sets default values for this component's properties
UDollStatComponent::UDollStatComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
	bWantsInitializeComponent = true;
}


// Called when the game starts
void UDollStatComponent::BeginPlay()
{
	Super::BeginPlay();

}

void UDollStatComponent::InitializeComponent()
{
	Super::InitializeComponent();
	auto myGameInstace = Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

	MYCHECK(myGameInstace != nullptr);
	StatData = myGameInstace->GetDollData();
	MYCHECK(StatData != nullptr);
}

float UDollStatComponent::GetAttackRange()
{
	return StatData->AttackRange;
}

float UDollStatComponent::GetAttackRadius()
{
	return StatData->AttackRadius;
}

float UDollStatComponent::GetHeartShareTime()
{
	return StatData->HeartShareTime;
}

float UDollStatComponent::GetWalkSpeed()
{
	return StatData->WalkSpeed;
}

float UDollStatComponent::GetMaxHP()
{
	return StatData->HP;
}

float UDollStatComponent::GetDetectRadius()
{
	return StatData->DetectRadius;
}