// Fill out your copyright notice in the Description page of Project Settings.

#include "Doll.h"
#include "DollAnimInstance.h"
#include "DollWeapon.h"
#include "DrawDebugHelpers.h"
#include "Common/HPComponent.h"
#include "Components/WidgetComponent.h"
#include "Doll/DollWeapon.h"
#include "Doll/DollAIController.h"

// Sets default values
ADoll::ADoll()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	HpComponent = CreateDefaultSubobject<UHPComponent>(TEXT("HpComponent"));

	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -88.0f), FRotator(0.0f, -90.0f, 0.0f));

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_CARDBOARD(TEXT("/Game/InfinityBladeWarriors/Character/CompleteCharacters/SK_CharM_Cardboard.SK_CharM_Cardboard"));

	if (SK_CARDBOARD.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(SK_CARDBOARD.Object);
	}

	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);

	static ConstructorHelpers::FClassFinder<UAnimInstance> WARRIOR_ANIM(TEXT("/Game/Blueprint/Doll/AB_DOLL.AB_DOLL_C"));

	if (WARRIOR_ANIM.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(WARRIOR_ANIM.Class);
	}

	GetCharacterMovement()->JumpZVelocity = 800.0f;

	IsAttacking = false;
	MaxCombo = 4;
	AttackEndComboState();

	AttackRange = 200.0f;
	AttackRadius = 50.0f;
	
	AIControllerClass = ADollAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

void ADoll::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	if (!IsPlayerControlled()) {
		GetCharacterMovement()->MaxWalkSpeed = 600.0f;
	}
}

// Called when the game starts or when spawned
void ADoll::BeginPlay()
{
	Super::BeginPlay();
	auto NewWeapon = GetWorld()->SpawnActor<ADollWeapon>(ADollWeapon::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator);
	SetWeapon(NewWeapon);
	HpComponent->SetMaxHP(100000.0f);
}

void ADoll::SetControlLocation(const FVector & location)
{
	auto dollAiController = Cast<ADollAIController>(GetController());
	MYCHECK(dollAiController != nullptr);
	dollAiController->SetControlLocation(location);
}

void ADoll::SetControlMoveState(bool isOn)
{
	auto dollAiController = Cast<ADollAIController>(GetController());
	MYCHECK(dollAiController != nullptr);
	dollAiController->SetControlMoveState(isOn);
}

bool ADoll::GetControlMoveState()
{
	auto dollAiController = Cast<ADollAIController>(GetController());
	return dollAiController->GetControlMoveState();
}

void ADoll::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	DollAnim = Cast<UDollAnimInstance>(GetMesh()->GetAnimInstance());
	MYCHECK(nullptr != DollAnim);

	DollAnim->OnMontageEnded.AddDynamic(this, &ADoll::OnAttackMontageEnded);

	DollAnim->OnNextAttackCheck.AddLambda([this]() -> void{
		CanNextCombo = false;

		if (IsComboInputOn)
		{
			AttackStartComboState();
			DollAnim->JumpToAttackMontageSection(CurrentCombo);
		}
	});

	DollAnim->OnAttackHitCheck.AddUObject(this, &ADoll::AttackCheck);

	HpComponent->OnDie.AddLambda([this]() -> void {
		DollAnim->SetDeadAnim();
		SetActorEnableCollision(false);
	});

}

float ADoll::TakeDamage(float DamageAmount, FDamageEvent const & DamageEvent, AController * EventInstigator, AActor * DamageCauser)
{
	return HpComponent->TakeDamage(0.0f);
}

bool ADoll::CanSetWeapon()
{
	return (nullptr == CurrentWeapon);
}

void ADoll::SetWeapon(ADollWeapon * NewWeapon)
{
	MYCHECK(nullptr != NewWeapon && nullptr == CurrentWeapon);
	FName WeaponSocket(TEXT("hand_rSocket"));
	if (nullptr != NewWeapon)
	{
		NewWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, WeaponSocket);
		NewWeapon->SetOwner(this);
		CurrentWeapon = NewWeapon;
	}
}

void ADoll::Attack()
{
	if (IsAttacking)
	{
		MYCHECK(FMath::IsWithinInclusive<int32>(CurrentCombo, 1, MaxCombo));
		if (CanNextCombo)
		{
			IsComboInputOn = true;
		}
	}
	else
	{
		MYCHECK(CurrentCombo == 0);
		AttackStartComboState();
		DollAnim->PlayAttackMontage();
		DollAnim->JumpToAttackMontageSection(CurrentCombo);
		IsAttacking = true;
	}
}

void ADoll::OnAttackMontageEnded(UAnimMontage * Montage, bool bInterrupted)
{
	MYCHECK(IsAttacking);
	MYCHECK(CurrentCombo > 0);
	IsAttacking = false;
	AttackEndComboState();
	OnAttackEnd.Broadcast();
}

void ADoll::AttackStartComboState()
{
	CanNextCombo = true;
	IsComboInputOn = false;
	MYCHECK(FMath::IsWithinInclusive<int32>(CurrentCombo, 0, MaxCombo - 1));
	CurrentCombo = FMath::Clamp<int32>(CurrentCombo + 1, 1, MaxCombo);
}

void ADoll::AttackEndComboState()
{
	IsComboInputOn = false;
	CanNextCombo = false;
	CurrentCombo = 0;
}

void ADoll::AttackCheck()
{
	FHitResult HitResult;
	FCollisionQueryParams Params(NAME_None, false, this);
	bool bResult = GetWorld()->SweepSingleByChannel(
		HitResult,
		GetActorLocation(),
		GetActorLocation() + GetActorForwardVector() * AttackRange,
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel2,
		FCollisionShape::MakeSphere(AttackRadius),
		Params);

#if ENABLE_DRAW_DEBUG

	FVector TraceVec = GetActorForwardVector() * AttackRange;
	FVector Center = GetActorLocation() + TraceVec * 0.5f;
	float HalfHeight = AttackRange * 0.5f + AttackRadius;
	FQuat CapsuleRot = FRotationMatrix::MakeFromZ(TraceVec).ToQuat();
	FColor DrawColor = bResult ? FColor::Green : FColor::Red;
	float DebugLifeTime = 5.0f;

	DrawDebugCapsule(GetWorld(),
		Center,
		HalfHeight,
		AttackRadius,
		CapsuleRot,
		DrawColor,
		false,
		DebugLifeTime);

#endif

	if (bResult)
	{
		if (HitResult.Actor.IsValid())
		{
			FDamageEvent DamageEvent;
			HitResult.Actor->TakeDamage(1.0f, DamageEvent, GetController(), this);
		}
	}
}