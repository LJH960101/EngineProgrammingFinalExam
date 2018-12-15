// Fill out your copyright notice in the Description page of Project Settings.

#include "Boss.h"
#include "Common/HPComponent.h"
#include "Monster/BossAIController.h"
#include "Monster/BossAnimInstance.h"
#include "Monster/BossStatComponent.h"
#include "Monster/BossTonado.h"
#include "Player/MyCharacter.h"
#include "Player/PC.h"
#include "Doll/Doll.h"
#include "DrawDebugHelpers.h"
#include "BrainComponent.h"
#include "NavigationSystem.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"

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

	static ConstructorHelpers::FObjectFinder<UParticleSystem> PS_JUMPING(TEXT("/Game/InfinityBladeEffects/Effects/FX_Skill_Leap/P_Skill_Leap_Fire_Crushing_Impact.P_Skill_Leap_Fire_Crushing_Impact"));
	if (PS_JUMPING.Succeeded()) {
		JumpingParticle = PS_JUMPING.Object;
	}
	else MYLOG(Warning, TEXT("Failed to find PS_JUMPING"));

	static ConstructorHelpers::FObjectFinder<UParticleSystem> PS_GATHERING(TEXT("/Game/InfinityBladeEffects/Effects/FX_Skill_Aura/P_AuraCircle_Default_IronSkin_01.P_AuraCircle_Default_IronSkin_01"));
	if (PS_GATHERING.Succeeded()) {
		GatheringParticle = PS_GATHERING.Object;
	}
	else MYLOG(Warning, TEXT("Failed to find PS_GATHERING"));

	static ConstructorHelpers::FObjectFinder<UParticleSystem> PS_GATHERING_FAIL(TEXT("/Game/InfinityBladeEffects/Effects/FX_Skill_Aura/P_Aura_Default_Upheaval_01.P_Aura_Default_Upheaval_01"));
	if (PS_GATHERING_FAIL.Succeeded()) {
		GatheringFailParticle = PS_GATHERING_FAIL.Object;
	}
	else MYLOG(Warning, TEXT("Failed to find PS_GATHERING_FAIL"));

	static ConstructorHelpers::FObjectFinder<UParticleSystem> PS_SCATTER(TEXT("/Game/InfinityBladeEffects/Effects/FX_Monsters/FX_Monster_Sentinel/P_Sentinel_Aggro_01.P_Sentinel_Aggro_01"));
	if (PS_SCATTER.Succeeded()) {
		ScatterParticle = PS_SCATTER.Object;
	}
	else MYLOG(Warning, TEXT("Failed to find PS_SCATTER"));

	static ConstructorHelpers::FObjectFinder<UParticleSystem> PS_SCATTER_IMPACT(TEXT("/Game/InfinityBladeEffects/Effects/FX_Monsters/FX_Monster_Sentinel/P_Sentinel_LeapSlam_Impact.P_Sentinel_LeapSlam_Impact"));
	if (PS_SCATTER_IMPACT.Succeeded()) {
		ScatterImpactParticle = PS_SCATTER_IMPACT.Object;
	}
	else MYLOG(Warning, TEXT("Failed to find PS_SCATTER_IMPACT"));

	static ConstructorHelpers::FObjectFinder<UParticleSystem> PS_HEARTSHARE(TEXT("/Game/InfinityBladeEffects/Effects/FX_Ability/Heal/P_Heal_Potion.P_Heal_Potion"));
	if (PS_HEARTSHARE.Succeeded()) {
		HeartShareParticle = PS_HEARTSHARE.Object;
	}
	else MYLOG(Warning, TEXT("Failed to find PS_HEARTSHARE"));

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
	HPComponent->SetMaxHP(300.0f);
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
	Cast<ABossAIController>(GetController())->GetBrainComponent()->StopLogic(TEXT("Dead"));
	UAIBlueprintHelperLibrary::SimpleMoveToActor(GetController(), this);
}

void ABoss::BindAnimationEvents()
{
	// 기본 공격 바인딩
	{
		MyAnim->BasicAttack.AddLambda([this]() -> void {
			const float height = 300.0f;
			const float radius = 300.0f;
			TArray<FHitResult> HitResults;
			FCollisionQueryParams Params(NAME_None, false, this);
			bool bResult = GetWorld()->SweepMultiByChannel(
				HitResults,
				GetActorLocation(),
				GetActorLocation() + GetActorForwardVector() * height,
				FQuat::Identity,
				ECollisionChannel::ECC_Pawn,
				FCollisionShape::MakeSphere(radius),
				Params);

			if (bResult)
			{
				for (auto HitResult : HitResults) {
					if (HitResult.Actor.IsValid())
					{
						if (Cast<IPC>(HitResult.Actor) == nullptr) continue;
						FDamageEvent DamageEvent;
						HitResult.Actor->TakeDamage(5.0f, DamageEvent, GetController(), this);
					}
				}
			}
		});
	}

	// 점프 공격 바인딩
	{
		MyAnim->Jumping.AddLambda([this]() -> void {
			TArray<AActor*> actors;
			AMyCharacter* playerCharacter = Cast<AMyCharacter>(UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetPawn());
			MYCHECK(playerCharacter != nullptr);
			FVector spawnLocation = playerCharacter->GetActorLocation();
			spawnLocation.Z = 0.f;
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), JumpingParticle, spawnLocation, FRotator::ZeroRotator, true);
			if (playerCharacter->GetCharacterMovement()->IsMovingOnGround()) {
				FDamageEvent DamageEvent;
				playerCharacter->TakeDamage(10.f, DamageEvent, GetController(), this);
			}
		}
		);
	}

	// 게더링 시작 바인딩
	{
		MyAnim->GatheringStart.AddLambda([this]() -> void {
			HPComponent->HealHP(10.0f);
			OnBossHpChanged.Broadcast();
			TArray<AActor*> actors;
			UGameplayStatics::GetAllActorsOfClass(GetWorld(), ADoll::StaticClass(), actors);
			for (auto actor : actors)
			{
				UGameplayStatics::SpawnEmitterAttached(GatheringParticle, actor->GetRootComponent(), TEXT("Particle"), FVector(0.0f, 0.0f, -50.0f), FRotator::ZeroRotator);
			}
		}
		);
	}

	// 게더링 종료 바인딩
	{
		MyAnim->GatheringEnd.AddLambda([this]() -> void {
			AMyCharacter* playerCharacter = Cast<AMyCharacter>(UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetPawn());
			MYCHECK(playerCharacter != nullptr);
			TArray<AActor*> actors;
			UGameplayStatics::GetAllActorsOfClass(GetWorld(), ADoll::StaticClass(), actors);
			MYCHECK(actors.Num() > 0);
			ADoll* doll = Cast<ADoll>(actors[0]);
			MYCHECK(doll != nullptr);
			float dist = FVector::Dist(doll->GetActorLocation(), playerCharacter->GetActorLocation());
			if (dist >= 500.f) {
				HPComponent->HealHP(50.0f);
				OnBossHpChanged.Broadcast();
				FDamageEvent newEvent;
				playerCharacter->TakeDamage(20.f, newEvent, GetController(), this);
				UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), GatheringFailParticle, playerCharacter->GetActorLocation());
			}
		}
		);
	}

	// 스캐터 시작 바인딩
	{
		MyAnim->ScatterStart.AddLambda([this]() -> void {
			TArray<AActor*> actors;
			UGameplayStatics::GetAllActorsOfClass(GetWorld(), ADoll::StaticClass(), actors);
			for (auto actor : actors)
			{
				UGameplayStatics::SpawnEmitterAttached(ScatterParticle, actor->GetRootComponent(), TEXT("Particle"), FVector(0.0f, 0.0f, 100.0f), FRotator::ZeroRotator);
			}
		}
		);
	}

	// 스캐터 종료 바인딩
	{
		MyAnim->ScatterEnd.AddLambda([this]() -> void {
			TArray<AActor*> actors;
			UGameplayStatics::GetAllActorsOfClass(GetWorld(), ADoll::StaticClass(), actors);
			FTransform newTransform = FTransform::Identity;
			newTransform.SetScale3D(FVector(5.0f, 5.0f, 5.0f));
			for (auto actor : actors)
			{
				newTransform.SetLocation(actor->GetActorLocation() - FVector(0.0f, 0.0f, 70.0f));
				UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ScatterImpactParticle,
					newTransform);
			}

			AMyCharacter* playerCharacter = Cast<AMyCharacter>(UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetPawn());
			MYCHECK(playerCharacter != nullptr);

			ADoll* doll = Cast<ADoll>(actors[0]);
			MYCHECK(doll != nullptr);

			float dist = FVector::Dist(doll->GetActorLocation(), playerCharacter->GetActorLocation());

			if (dist <= 800.f) {
				FDamageEvent newEvent;
				playerCharacter->TakeDamage(20.f, newEvent, GetController(), this);
			}
		}
		);
	}

	// 위치변환기 바인딩
	{
		MyAnim->TransLocation.AddLambda([this]() -> void {
			TArray<AActor*> actors;
			UGameplayStatics::GetAllActorsOfClass(GetWorld(), ADoll::StaticClass(), actors);

			AMyCharacter* playerCharacter = Cast<AMyCharacter>(UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetPawn());
			MYCHECK(playerCharacter != nullptr);

			ADoll* doll = Cast<ADoll>(actors[0]);
			MYCHECK(doll != nullptr);
			
			FVector dollLocation = doll->GetActorLocation();
			doll->SetActorLocation(playerCharacter->GetActorLocation());
			playerCharacter->SetActorLocation(dollLocation);

			auto aiController = Cast<ABossAIController>(GetController());
			MYCHECK(aiController != nullptr);
			aiController->TransTarget();
		}
		);
	}

	// 토네이도 바인딩
	{
		MyAnim->Tonado.AddLambda([this]() -> void {
			GetWorld()->SpawnActor<ABossTonado>(ABossTonado::StaticClass(), GetActorLocation(), FRotator::ZeroRotator);
		}
		);
	}

	// 체력 쉐어 바인딩
	{
		MyAnim->HeartShare.AddLambda([this]() -> void {
			AMyCharacter* playerCharacter = Cast<AMyCharacter>(UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetPawn());
			MYCHECK(playerCharacter != nullptr);

			TArray<AActor*> actors;
			UGameplayStatics::GetAllActorsOfClass(GetWorld(), ADoll::StaticClass(), actors);
			for (auto actor : actors)
			{
				UGameplayStatics::SpawnEmitterAttached(HeartShareParticle, actor->GetRootComponent(), TEXT("Particle"), FVector(0.0f, 0.0f, 150.0f), FRotator::ZeroRotator);
				auto doll = Cast<ADoll>(actor);
				if (doll != nullptr) doll->ActiveHeartShare();
			}
			UGameplayStatics::SpawnEmitterAttached(HeartShareParticle, playerCharacter->GetRootComponent(), TEXT("Particle"), FVector(0.0f, 0.0f, 150.0f), FRotator::ZeroRotator);
		}
		);
	}
}
void ABoss::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	MyAnim = Cast<UBossAnimInstance>(GetMesh()->GetAnimInstance());
	MYCHECK(nullptr != MyAnim);

	// 죽음 함수 바인딩
	HPComponent->OnDie.AddUObject(this, &ABoss::Die);

	BindAnimationEvents();
}

float ABoss::TakeDamage(float DamageAmount, FDamageEvent const & DamageEvent, AController * EventInstigator, AActor * DamageCauser)
{
	float hp = HPComponent->TakeDamage(DamageAmount);
	OnBossHpChanged.Broadcast();
	return hp;
}
