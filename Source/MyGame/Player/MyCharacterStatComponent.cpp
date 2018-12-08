// Fill out your copyright notice in the Description page of Project Settings.

#include "MyCharacterStatComponent.h"
#include "Player/MyCharacter.h"
#include "GameEssencil/MyGameInstance.h"

// Sets default values for this component's properties
UMyCharacterStatComponent::UMyCharacterStatComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
	bWantsInitializeComponent = true;

	// ...
}


// Called when the game starts
void UMyCharacterStatComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

void UMyCharacterStatComponent::InitializeComponent() 
{
	Super::InitializeComponent();
	auto myGameInstace = Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

	MYCHECK(myGameInstace != nullptr);
	StatData = myGameInstace->GetMyCharacterData();
	MYCHECK(StatData != nullptr);
	_hp = StatData->PlayerHP;

	auto myCharacter = Cast<AMyCharacter>(GetOwner());
	if (myCharacter != nullptr) myCharacter->GetCharacterMovement()->JumpZVelocity = GetJumpPower();
}


// Called every frame
void UMyCharacterStatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

float UMyCharacterStatComponent::TakeDamage(float damage)
{
	_hp = FMath::Clamp(_hp - damage, 0.0f, StatData->PlayerHP);
	OnHPChanged.Broadcast();
	if (_hp <= KINDA_SMALL_NUMBER) OnDie.Broadcast();
	return _hp;
}

float UMyCharacterStatComponent::GetHp()
{
	return _hp;
}

float UMyCharacterStatComponent::GetHpRate()
{
	return _hp / StatData->PlayerHP;
}

float UMyCharacterStatComponent::GetAttackDamage()
{
	return StatData->PlayerAttackDamage;
}

float UMyCharacterStatComponent::GetDashDamage()
{
	return StatData->PlayerDashDamage;
}

float UMyCharacterStatComponent::GetBulletSpeed()
{
	return StatData->PlayerBulletSpeed;
}

float UMyCharacterStatComponent::GetDashRange()
{
	return StatData->PlayerDashRange;
}

float UMyCharacterStatComponent::GetJumpPower()
{
	return StatData->PlayerJumpPower;
}

float UMyCharacterStatComponent::GetDashRadiusRange()
{
	return StatData->PlayerDashRadiusRange;
}

