// Fill out your copyright notice in the Description page of Project Settings.

#include "MyCharacter.h"
#include "Player/MyBullet.h"
#include "Components/WidgetComponent.h"
#include "Player/MyCharacterAnimInstance.h"
#include "GameEssencil/MyGameInstance.h"
#include "GameFramework//DamageType.h"
#include "Player/MyCharacterStatComponent.h"
#include "Player/MyCharacterHPWidget.h"

// Sets default values
AMyCharacter::AMyCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	// ������Ʈ ����
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CharacterStat = CreateDefaultSubobject<UMyCharacterStatComponent>(TEXT("CharacterStat"));
	HPBarWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("HpBarWidget"));

	// ���� ����
	SpringArm->SetupAttachment(GetCapsuleComponent());
	Camera->SetupAttachment(SpringArm);
	HPBarWidget->SetupAttachment(GetMesh());

	// ������Ʈ ����
	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -88.0f),
		FRotator(0.0f, -90.0f, 0.0f));
	
	SpringArm->TargetArmLength = 250.0f;
	SpringArm->SocketOffset = FVector(0.0f, 100.0f, 50.0f);
	SpringArm->SetRelativeRotation(FRotator(-15.0f, 0.0f, 0.0f));
	SpringArm->bUsePawnControlRotation = true;
	SpringArm->bInheritPitch = true;
	SpringArm->bInheritRoll = false;
	SpringArm->bInheritYaw = true;
	SpringArm->bDoCollisionTest = true;
	bUseControllerRotationYaw = false;

	HPBarWidget->SetRelativeLocation(FVector(0.0f, 0.0f, 180.0f));
	HPBarWidget->SetWidgetSpace(EWidgetSpace::Screen);
	static ConstructorHelpers::FClassFinder<UUserWidget> UI_HPBAR(TEXT("/Game/Widget/CharacterWidget.CharacterWidget_C"));
	if (UI_HPBAR.Succeeded()) {
		HPBarWidget->SetWidgetClass(UI_HPBAR.Class);
		HPBarWidget->SetDrawSize(FVector2D(150.0f, 50.0f));
	}
	else MYLOG(Warning, TEXT("Failed to load UI_HPBAR"));

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 720.0f, 0.0f);

	// ������Ʈ�� ����
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_PHASE(TEXT("/Game/ParagonPhase/Characters/Heroes/Phase/Meshes/Phase_GDC.Phase_GDC"));
	if (SK_PHASE.Succeeded()) {
		GetMesh()->SetSkeletalMesh(SK_PHASE.Object);
	}
	else MYLOG(Warning, TEXT("Failed to find SK_PHASE"));

	static ConstructorHelpers::FClassFinder<UAnimInstance> AB_PHASE(TEXT("/Game/Blueprint/AB_Phase.AB_Phase_C"));
	if (AB_PHASE.Succeeded()) {
		GetMesh()->SetAnimInstanceClass(AB_PHASE.Class);
	}
	else MYLOG(Warning, TEXT("Failed to find AB_PHASE"));
	
	// �ʵ带 ����
	IsAttacking = false;
	IsDashing = false;
	IsDie = false;
	MaxCombo = 4;
	AttackEndComboState();
}

float AMyCharacter::GetHPRate()
{
	return CharacterStat->GetHpRate();
}

void AMyCharacter::Die()
{
	IsDie = true;
}

void AMyCharacter::MoveForward(float axis)
{
	if (IsDashing || IsDie) return;
	AddMovementInput(FRotationMatrix(GetControlRotation()).GetUnitAxis(EAxis::X), axis);
}

void AMyCharacter::MoveRight(float axis)
{
	if (IsDashing || IsDie) return;
	AddMovementInput(FRotationMatrix(GetControlRotation()).GetUnitAxis(EAxis::Y), axis);
}

void AMyCharacter::Turn(float axis)
{
	AddControllerYawInput(axis);
}

void AMyCharacter::LookUp(float axis)
{
	AddControllerPitchInput(axis);
}

void AMyCharacter::Attack()
{
	if (IsDashing || IsDie) return;
	if (IsAttacking) {
		MYCHECK(FMath::IsWithinInclusive<int32>(CurrentCombo, 1, MaxCombo));
		if (CanNextCombo) {
			IsComboInputOn = true;
		}
	}
	else {
		MYCHECK(CurrentCombo == 0);
		FRotator PlayerRot = ((GetActorLocation()+ FRotationMatrix(GetControlRotation()).GetUnitAxis(EAxis::X)) - GetActorLocation()).Rotation();
		PlayerRot.Pitch = 0.0f;
		PlayerRot.Roll = 0.0f;
		SetActorRotation(PlayerRot);
		AttackStartComboState();
		MyAnim->PlayerAttackMontage();
		MyAnim->JumpToAttackMontageSection(CurrentCombo);
		IsAttacking = true;
	}
}

void AMyCharacter::OnAttackMontageEnded(UAnimMontage * Montage, bool bInterrupted)
{
	MYCHECK(IsAttacking);
	MYCHECK(CurrentCombo > 0);
	IsAttacking = false;
	AttackEndComboState();
}

void AMyCharacter::AttackStartComboState()
{
	CanNextCombo = true;
	IsComboInputOn = false;
	MYCHECK(FMath::IsWithinInclusive<int32>(CurrentCombo, 0, MaxCombo - 1));
	CurrentCombo = FMath::Clamp<int32>(CurrentCombo + 1, 1, MaxCombo);
}

void AMyCharacter::AttackEndComboState()
{
	IsComboInputOn = false;
	CanNextCombo = false;
	CurrentCombo = 0;
}

// Called when the game starts or when spawned
void AMyCharacter::BeginPlay()
{
	Super::BeginPlay();

	auto CharacterWidget = Cast<UMyCharacterHPWidget>(HPBarWidget->GetUserWidgetObject());
	if (nullptr != CharacterWidget) {
		CharacterWidget->BindCharacterStat(CharacterStat);
	}
}

// Called every frame
void AMyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AMyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// �Է��� ���ε�
	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &AMyCharacter::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &AMyCharacter::MoveRight);
	PlayerInputComponent->BindAxis(TEXT("Turn"), this, &AMyCharacter::Turn);
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &AMyCharacter::LookUp);
	PlayerInputComponent->BindAction(TEXT("Jump"), EInputEvent::IE_Pressed, this, &AMyCharacter::Jump);
	PlayerInputComponent->BindAction(TEXT("Attack"), EInputEvent::IE_Pressed, this, &AMyCharacter::Attack);
	PlayerInputComponent->BindAction(TEXT("Dash"), EInputEvent::IE_Pressed, this, &AMyCharacter::Dash);
	PlayerInputComponent->BindAction(TEXT("Test"), EInputEvent::IE_Pressed, this, &AMyCharacter::Test);
}

float AMyCharacter::TakeDamage(float DamageAmount, FDamageEvent const & DamageEvent, AController * EventInstigator, AActor * DamageCauser)
{
	return CharacterStat->TakeDamage(DamageAmount);
}

void AMyCharacter::Test()
{
	CharacterStat->TakeDamage(5.0f);
}

void AMyCharacter::Dash()
{
	if (IsDashing || IsDie) return;
	IsAttacking = false;
	IsDashing = true;
	AttackEndComboState();
	FVector moveDir = FRotationMatrix(GetControlRotation()).GetUnitAxis(EAxis::X);
	moveDir.Normalize();
	moveDir.Z = 0.0f;
	SetActorLocation(GetActorLocation() + moveDir * CharacterStat->GetDashRange());
	TArray<AActor*> ignoreActors;
	ignoreActors.Add(this);
	UGameplayStatics::ApplyRadialDamage(GetWorld(), CharacterStat->GetDashDamage(), GetActorLocation(), CharacterStat->GetDashRadiusRange(), UDamageType::StaticClass(), ignoreActors);
}

bool AMyCharacter::OnDash()
{
	return IsDashing;
}

bool AMyCharacter::OnDie()
{
	return IsDie;
}

void AMyCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	MyAnim = Cast<UMyCharacterAnimInstance>(GetMesh()->GetAnimInstance());
	MYCHECK(nullptr != MyAnim);

	MyAnim->OnMontageEnded.AddDynamic(this, &AMyCharacter::OnAttackMontageEnded);


	// ���� ���� ���� ĳ����
	MyAnim->OnNextAttackCheck.AddLambda([this]() -> void {
		CanNextCombo = false;

		if (IsComboInputOn) {
			AttackStartComboState();
			MyAnim->JumpToAttackMontageSection(CurrentCombo);
		}
	});

	// ���� �Լ� ���ε�
	CharacterStat->OnDie.AddUObject(this, &AMyCharacter::Die);

	// ���� ���� ���ε�
	MyAnim->OnAttack.AddLambda([this]() -> void {
		FVector spawnLocation = GetActorLocation() + GetActorForwardVector() * 100.0f;

		AMyBullet* myBullet = GetWorld()->SpawnActor<AMyBullet>(AMyBullet::StaticClass(), spawnLocation, GetControlRotation());
		myBullet->SetOwner(this);
		myBullet->SetSpeed(this->CharacterStat->GetBulletSpeed());
		myBullet->SetDamage(this->CharacterStat->GetAttackDamage());
		myBullet->GetSphereCP()->IgnoreActorWhenMoving(this, true);
	});

	// ��� ���� ���� ���ε�
	MyAnim->OnDashEnd.AddLambda([this]() -> void {
		IsDashing = false;
	});
}