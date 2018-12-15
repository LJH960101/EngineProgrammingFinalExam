// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "MyGame.h"
#include "GameFramework/Actor.h"
#include "BossTonado.generated.h"

UCLASS()
class MYGAME_API ABossTonado : public AActor
{
	GENERATED_BODY()

	// Field
private:
	UPROPERTY(VisibleAnywhere)
	UParticleSystemComponent* ParticleSystem;
	UPROPERTY(VisibleAnywhere)
	class UProjectileMovementComponent* ProjectileMovementComponent;
	UPROPERTY()
	UParticleSystem* ExprosionParticle;
	float attackTimer = 0.5f;

	AActor* PlayerActor;

	// Method
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
public:	
	// Sets default values for this actor's properties
	ABossTonado();
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
