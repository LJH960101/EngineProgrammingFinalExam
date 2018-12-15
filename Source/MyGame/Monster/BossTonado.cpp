// Fill out your copyright notice in the Description page of Project Settings.

#include "BossTonado.h"
#include "Engine/World.h"
#include "Player/MyPlayerController.h"
#include "Player/MyCharacter.h"
#include "Player/PC.h"
#include "Monster/Boss.h"
#include "GameFramework/ProjectileMovementComponent.h"

// Sets default values
ABossTonado::ABossTonado()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// 컴포넌트 생성
	ParticleSystem = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Particle Emitter"));
	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile Movement Component"));

	// 계층 설정
	RootComponent = ParticleSystem;

	// 오브젝트 설정
	static ConstructorHelpers::FObjectFinder<UParticleSystem> PT_ICESTORM(TEXT("/Game/InfinityBladeEffects/Effects/FX_Mobile/ICE/P_IceWall_SpawnLoop.P_IceWall_SpawnLoop"));
	if (PT_ICESTORM.Succeeded()) {
		ParticleSystem->SetTemplate(PT_ICESTORM.Object);
	}
	else MYLOG(Warning, TEXT("Failed to PT_ICESTORM."));

	// 컴포넌트 설정
	ProjectileMovementComponent->InitialSpeed = 0.0f;
	ProjectileMovementComponent->ProjectileGravityScale = 0.0f;
}

// Called when the game starts or when spawned
void ABossTonado::BeginPlay()
{
	Super::BeginPlay();
	PlayerActor = UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetPawn();
	SetActorScale3D(FVector(2.0f, 2.0f, 2.0f));
}

const float Damage = 5.0f;
const float AttackTime = 0.5f;
const float height = 150.0f;
const float radius = 150.0f;
const float speed = 200.0f;
// Called every frame
void ABossTonado::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	attackTimer -= DeltaTime;
	// 공격할 시간!
	if (attackTimer <= 0.0f)
	{
		attackTimer += AttackTime;

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
					if(Cast<IPC>(HitResult.Actor) == nullptr) continue;
					FDamageEvent DamageEvent;
					HitResult.Actor->TakeDamage(Damage, DamageEvent, nullptr, this);
				}
			}
		}
	}

	// 주인공 추적
	FVector newVec = (PlayerActor->GetActorLocation() - GetActorLocation());
	newVec.Normalize();
	ProjectileMovementComponent->Velocity = newVec * speed;

	// 몬스터가 죽었는지 판단함.
	TArray<AActor*> bosses;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABoss::StaticClass(), bosses);
	if (Cast<ABoss>(bosses[0])->OnDie()) Destroy();
}