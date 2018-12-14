// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "MyGame.h"
#include "GameFramework/Actor.h"
#include "MyBullet.generated.h"

UCLASS()
class MYGAME_API AMyBullet : public AActor
{
	GENERATED_BODY()
	
	// Field
private:
	UPROPERTY(VisibleAnywhere)
	UParticleSystemComponent* ParticleSystem;
	UPROPERTY(VisibleAnywhere)
	USphereComponent* SphereComponent;
	UPROPERTY(VisibleAnywhere)
	class UProjectileMovementComponent* ProjectileMovementComponent;
	UPROPERTY()
	UParticleSystem* ExprosionParticle;
	float _damage = 1.0f;
	APawn* owner;
	UPROPERTY()
	class USoundWave* ExpSoundWave;

	// Method
public:	
	// Sets default values for this actor's properties
	AMyBullet();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SetDamage(float damage);
	void SetOwnerPlayer(APawn* owningActor);
	void SetSpeed(float speed);
	USphereComponent* GetSphereCP();

private:
	UFUNCTION()
	void OnComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

};
