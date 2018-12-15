// Fill out your copyright notice in the Description page of Project Settings.

#include "Boss.h"
#include "Common/HPComponent.h"
#include "Monster/BossAIController.h"
#include "Monster/BossAnimInstance.h"
#include "Monster/BossStatComponent.h"
#include "DrawDebugHelpers.h"

// Sets default values
ABoss::ABoss()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StatComponent = CreateDefaultSubobject<UBossStatComponent>(TEXT("StatComponent"));
	HPComponent = CreateDefaultSubobject<UHPComponent>(TEXT("HpComponent"));

	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -88.0f),
		FRotator(0.0f, -90.0f, 0.0f));
	SetActorScale3D(FVector(3.0f, 3.0f, 3.0f));

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 720.0f, 0.0f);
	GetCharacterMovement()->MaxWalkSpeed = 900.0f;

	// Object Setting
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_BORIS(TEXT("/Game/ParagonBoris/Characters/Heroes/Boris/Meshes/Boris.Boris"));
	if (SK_BORIS.Succeeded()) {
		GetMesh()->SetSkeletalMesh(SK_BORIS.Object);
	}
	else MYLOG(Warning, TEXT("Failed to find SK_BORIS"));

	static ConstructorHelpers::FClassFinder<UAnimInstance> AB_BORIS(TEXT("/Game/Blueprint/Boris/AB_Boss.AB_Boss_C"));
	if (AB_BORIS.Succeeded()) {
		GetMesh()->SetAnimInstanceClass(AB_BORIS.Class);
	}
	else MYLOG(Warning, TEXT("Failed to find AB_BORIS"));

	AIControllerClass = ABossAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	bOnAttack = false;
}

bool ABoss::OnAttack()
{
	return bOnAttack;
}

void ABoss::SetOnAttack(bool isOn)
{
	bOnAttack = isOn;
	auto controller = Cast<ABossAIController>(GetController());
	MYCHECK(controller != nullptr);
	controller->ChangeOnAttack(isOn);
}

void ABoss::SetPattern(EBossPattern pattern)
{
	CurrentPattern = pattern;
	SetOnAttack(true);
	auto controller = Cast<ABossAIController>(GetController());
	MYCHECK(controller != nullptr);
	controller->SetCurrentState(pattern);
}

EBossPattern ABoss::GetCurrentPattern()
{
	return CurrentPattern;
}

// Called when the game starts or when spawned
void ABoss::BeginPlay()
{
	Super::BeginPlay();
	HPComponent->SetMaxHP(100.0f);
}

// Called every frame
void ABoss::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

bool ABoss::OnDie()
{
	return IsDie;
}

void ABoss::Die()
{
	IsDie = true;
	SetPattern(EBossPattern::BP_NotAttacking);
}

void ABoss::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	MyAnim = Cast<UBossAnimInstance>(GetMesh()->GetAnimInstance());
	MYCHECK(nullptr != MyAnim);

	// 죽음 함수 바인딩
	HPComponent->OnDie.AddUObject(this, &ABoss::Die);

	// 기본 공격 바인딩
	MyAnim->BasicAttack.AddLambda([this]() -> void {
		const float height = 300.0f;
		const float radius = 300.0f;
		MYLOG_S(Warning);
		FHitResult HitResult;
		FCollisionQueryParams Params(NAME_None, false, this);
		bool bResult = GetWorld()->SweepSingleByChannel(
			HitResult,
			GetActorLocation(),
			GetActorLocation() + GetActorForwardVector() * height,
			FQuat::Identity,
			ECollisionChannel::ECC_Pawn,
			FCollisionShape::MakeSphere(radius),
			Params);

#if ENABLE_DRAW_DEBUG

		FVector TraceVec = GetActorForwardVector() * height;
		FVector Center = GetActorLocation() + TraceVec * 0.5f;
		float HalfHeight = height * 0.5f + radius;
		FQuat CapsuleRot = FRotationMatrix::MakeFromZ(TraceVec).ToQuat();
		FColor DrawColor = bResult ? FColor::Green : FColor::Red;
		float DebugLifeTime = radius;

		DrawDebugCapsule(GetWorld(),
			Center,
			HalfHeight,
			radius,
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
				HitResult.Actor->TakeDamage(5.0f, DamageEvent, GetController(), this);
			}
		}
	});
}

float ABoss::TakeDamage(float DamageAmount, FDamageEvent const & DamageEvent, AController * EventInstigator, AActor * DamageCauser)
{
	MYLOG(Warning, TEXT("%f"), HPComponent->GetHp());
	return HPComponent->TakeDamage(DamageAmount);
}
