// Fill out your copyright notice in the Description page of Project Settings.

#include "MyBullet.h"
#include "Engine/World.h"
#include "Monster/NPC.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "GameFramework/GameSession.h"

// Sets default values
AMyBullet::AMyBullet()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// ������Ʈ ����
	ParticleSystem = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Particle Emitter"));
	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere Component"));
	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile Movement Component"));

	// ���� ����
	RootComponent = SphereComponent;
	ParticleSystem->SetupAttachment(SphereComponent);

	// ������Ʈ ����
	static ConstructorHelpers::FObjectFinder<UParticleSystem> PT_FIREBALL(TEXT("/Game/InfinityBladeEffects/Effects/FX_Mobile/Fire/combat/P_FireBall_Strong.P_FireBall_Strong"));
	if (PT_FIREBALL.Succeeded()) {
		ParticleSystem->SetTemplate(PT_FIREBALL.Object);
	}
	else MYLOG(Warning, TEXT("Failed to PT_FIREBALL."));

	static ConstructorHelpers::FObjectFinder<UParticleSystem> PT_EXPLOSION(TEXT("/Game/InfinityBladeEffects/Effects/FX_Treasure/Resources/P_OrePile_Explosion_Basic.P_OrePile_Explosion_Basic"));
	ExprosionParticle = PT_EXPLOSION.Object;

	// ������Ʈ ����
	ProjectileMovementComponent->InitialSpeed = 1000.0f;
	ProjectileMovementComponent->ProjectileGravityScale = 0.0f;
	
	SphereComponent->SetEnableGravity(false);
	SphereComponent->SetCollisionProfileName(FName("BlockAll"));
	SphereComponent->OnComponentHit.AddDynamic(this, &AMyBullet::OnComponentHit);
}

// Called when the game starts or when spawned
void AMyBullet::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AMyBullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMyBullet::SetDamage(float damage) {
	_damage = damage;
}

void AMyBullet::SetSpeed(float speed)
{
	ProjectileMovementComponent->InitialSpeed = speed;
}

void AMyBullet::SetOwner(APawn* owningActor)
{
	owner = owningActor;
}

USphereComponent* AMyBullet::GetSphereCP()
{
	return SphereComponent;
}

void AMyBullet::OnComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	auto Pawn = Cast<APawn>(OtherActor);
	if (Pawn != nullptr) {
		if (Cast<INPC>(OtherActor) != nullptr) {
			// ���Ͱ� �¾Ҵٸ� ���ظ� ������.
			if (owner == nullptr) return;

			FDamageEvent DamageEvent;
			OtherActor->TakeDamage(_damage, DamageEvent, owner->GetController(), this);
		}
	}

	// ��ƼŬ�� ��Ʈ���� ������.
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExprosionParticle, Hit.Location, GetActorRotation(), true);
	Destroy();
}