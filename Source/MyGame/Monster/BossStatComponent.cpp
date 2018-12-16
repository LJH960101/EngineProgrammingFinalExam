// Fill out your copyright notice in the Description page of Project Settings.

#include "BossStatComponent.h"
#include "GameEssencil/MyGameInstance.h"

// Sets default values for this component's properties
UBossStatComponent::UBossStatComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
	bWantsInitializeComponent = true;
}


// Called when the game starts
void UBossStatComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UBossStatComponent::InitializeComponent()
{

	Super::InitializeComponent();
	auto myGameInstace = Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

	MYCHECK(myGameInstace != nullptr);
	StatData = myGameInstace->GetBossData();
	MYCHECK(StatData != nullptr);
}

float UBossStatComponent::GetMaxHp()
{
	return StatData->HP;
}
float UBossStatComponent::GetDetectRadius()
{
	return StatData->DetectRadius;
}
float UBossStatComponent::GetPatternRate()
{
	return StatData->PatternRate;
}
float UBossStatComponent::GetWalkSpeed()
{
	return StatData->WalkSpeed;
}
float UBossStatComponent::GetBasicAttackHeight()
{
	return StatData->BasicAttackHeight;
}
float UBossStatComponent::GetBasicAttackRadius()
{
	return StatData->BasicAttackRadius;
}
float UBossStatComponent::GetBasicAttackDamage()
{
	return StatData->BasicAttackDamage;
}
float UBossStatComponent::GetJumpingDamage()
{
	return StatData->JumpingDamage;
}
float UBossStatComponent::GetGatheringStartHeal()
{
	return StatData->GatheringStartHeal;
}
float UBossStatComponent::GetGatheringEndHeal()
{
	return StatData->GatheringEndHeal;
}
float UBossStatComponent::GetGatheringDist()
{
	return StatData->GatheringDist;
}
float UBossStatComponent::GetGatheringDamage()
{
	return StatData->GatheringDamage;
}
float UBossStatComponent::GetScatterDist()
{
	return StatData->ScatterDist;
}
float UBossStatComponent::GetScatterDamage()
{
	return StatData->ScatterDamage;
}