// Fill out your copyright notice in the Description page of Project Settings.


#include "SlAiPlayerCharacter.h"
#include "ConstructorHelpers.h"
#include "Engine/SkeletalMesh.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/Controller.h"
#include "RotationMatrix.h"
#include "QuatRotationTranslationMatrix.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/ChildActorComponent.h"

#include "SlAiHandObject.h"
#include "SlAiHelper.h"
#include "SlAiFlobObject.h"
#include "SlAiPackageManager.h"
#include "SlAiPlayerController.h"
#include "SlAiPlayerState.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ASlAiPlayerCharacter::ASlAiPlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//����������ײ����ΪplayerProfile�������meshģ�͵���ײ�Ϳ�������������ײ
	GetCapsuleComponent()->SetCollisionProfileName(FName("PlayerProfile"));

	//��ӵ�һ�˳ƹ���ģ��
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> StaticMeshFirst(TEXT("SkeletalMesh'/Game/Res/PolygonAdventure/Mannequin/FirstPlayer/SkMesh/FirstPlayer.FirstPlayer'"));
	MeshFirst = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MeshFirst"));
	MeshFirst->SetSkeletalMesh(StaticMeshFirst.Object);
	MeshFirst->SetupAttachment((USceneComponent*)GetCapsuleComponent());
	MeshFirst->bOnlyOwnerSee = true;
	MeshFirst->bCastDynamicShadow = false;
	MeshFirst->bReceivesDecals = false;
	//����Ƶ��˥��
	MeshFirst->VisibilityBasedAnimTickOption = EVisibilityBasedAnimTickOption::OnlyTickPoseWhenRendered;
	MeshFirst->PrimaryComponentTick.TickGroup = TG_PrePhysics;
	//������ײ����
	MeshFirst->SetCollisionObjectType(ECC_Pawn);
	MeshFirst->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	MeshFirst->SetCollisionResponseToAllChannels(ECR_Ignore);
	//����λ��
	MeshFirst->SetRelativeLocation(FVector(0.f, 0.f, -90.f));
	MeshFirst->SetRelativeRotation(FQuat::MakeFromEuler(FVector(0.0f, 0.f, -90.f)));

	//��ȡ��һ�˳ƵĶ�����ͼ
	//����Ƕ�����ͼ��Ctrl C��Ctrl V���·����Ҫ����_C
	static ConstructorHelpers::FClassFinder<UAnimInstance> StaticAnimFirst(TEXT("AnimBlueprint'/Game/Blueprint/Player/FirstPlayer_Animation.FirstPlayer_Animation_C'"));
	MeshFirst->AnimClass = StaticAnimFirst.Class;

	//��Ĭ��Mesh��ӹ���ģ��
	//ģ�͵�·����ȡ��unity�༭�����棬�ҵ���Ӧ��ģ���ļ���Ȼ��Ctrl+C���ƾͿ���
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> StaticMeshThird(TEXT("SkeletalMesh'/Game/Res/PolygonAdventure/Mannequin/Player/SkMesh/Player.Player'"));
	GetMesh()->SetSkeletalMesh(StaticMeshThird.Object);
	GetMesh()->bOnlyOwnerSee = true;
	GetMesh()->bReceivesDecals = false;
	GetMesh()->SetCollisionObjectType(ECC_Pawn);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetMesh()->SetCollisionResponseToAllChannels(ECR_Ignore);
	GetMesh()->SetRelativeLocation(FVector(0.f, 0.f, -90.f));
	GetMesh()->SetRelativeRotation(FQuat::MakeFromEuler(FVector(0.0f, 0.f, -90.f)));

	//��ȡ�����˳ƵĶ�����ͼ
	//����Ƕ�����ͼ��Ctrl C��Ctrl V���·����Ҫ����_C
	static ConstructorHelpers::FClassFinder<UAnimInstance> StaticAnimThird(TEXT("AnimBlueprint'/Game/Blueprint/Player/ThirdPlayer_Animation.ThirdPlayer_Animation_C'"));
	GetMesh()->AnimClass = StaticAnimThird.Class;

	//������ֱ�
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	//���þ���
	CameraBoom->TargetArmLength = 300.f;
	//����ƫ��
	CameraBoom->TargetOffset = FVector(0.f, 0.f, 60.f);
	//��controller����ת
	CameraBoom->bUsePawnControlRotation = true;

	//��ʼ�������˳������
	ThirdCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("ThirdCamera"));
	ThirdCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	//����ThirdCamera���������������ת
	ThirdCamera->bUsePawnControlRotation = false;

	//��ʼ����һ�������
	FirstCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstCamera"));
	FirstCamera->SetupAttachment((USceneComponent*)GetCapsuleComponent());
	//���ø���controller����ת
	FirstCamera->bUsePawnControlRotation = true;
	//����λ��
	FirstCamera->AddLocalOffset(FVector(0.f, 0.f, 60.f));

	//Ĭ�ϵ����˳�
	FirstCamera->SetActive(false);
	ThirdCamera->SetActive(true);
	//����ʾ��һ�˳�ģ��
	GetMesh()->SetOwnerNoSee(false);
	MeshFirst->SetOwnerNoSee(true);

	//ʵ����������Ʒ
	HandObject = CreateDefaultSubobject<UChildActorComponent>(TEXT("HandObject"));

	//��ʼ������
	BaseLookUpRate = 45.f;
	BaseTurnRate = 45.f;
	//���ó�ʼ�ٶ�Ϊ150
	GetCharacterMovement()->MaxWalkSpeed = 150.f;
	//��ʼ���������ӽ�
	GameView = EGameViewMode::Third;
	//�ϰ�������ʼ��Ϊ�޶���
	UpperType = EUpperBody::None;
	//һ��ʼ�����л��ӽ�
	IsAllowSwitch = true;

	IsInputLocked = false;
	//��ʼ��û�й���
	IsAttack = false;
}

// Called when the game starts or when spawned
void ASlAiPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	SPController = Cast<ASlAiPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));

	//���ֳ���Ʒ����󶨵�������ģ�����ֲ����
	HandObject->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName("RHSocket"));

	//���Actor��HandeObject
	HandObject->SetChildActorClass(ASlAiHandObject::SpawnHandObject(0));
}

// Called every frame
void ASlAiPlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ASlAiPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	check(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &ASlAiPlayerCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ASlAiPlayerCharacter::MoveRight);
	PlayerInputComponent->BindAxis("Turn", this, &ASlAiPlayerCharacter::Turn);
	PlayerInputComponent->BindAxis("LookUp", this, &ASlAiPlayerCharacter::LookUpAtRate);
	PlayerInputComponent->BindAxis("TurnRate", this, &ASlAiPlayerCharacter::TurnAtRate);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ASlAiPlayerCharacter::OnStartJump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ASlAiPlayerCharacter::OnStopJump);
	PlayerInputComponent->BindAction("Run", IE_Pressed, this, &ASlAiPlayerCharacter::OnStartRun);
	PlayerInputComponent->BindAction("Run", IE_Released, this, &ASlAiPlayerCharacter::OnStopRun);
}

void ASlAiPlayerCharacter::ChangeView(EGameViewMode::Type NewGameView)
{
	GameView = NewGameView;
	switch (GameView)
	{
	case EGameViewMode::First:
		FirstCamera->SetActive(true);
		ThirdCamera->SetActive(false);
		MeshFirst->SetOwnerNoSee(false);
		GetMesh()->SetOwnerNoSee(true);
		//�޸�handobject��λ��
		HandObject->AttachToComponent(MeshFirst, FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName("RHSocket"));
		break;
	case EGameViewMode::Third:
		FirstCamera->SetActive(false);
		ThirdCamera->SetActive(true);
		MeshFirst->SetOwnerNoSee(true);
		GetMesh()->SetOwnerNoSee(false);
		//�޸�handobject��λ��
		HandObject->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName("RHSocket"));
		break;
	}
}

void ASlAiPlayerCharacter::ChangeHandObject(TSubclassOf<class AActor> HandObjectClass)
{
	//������Ʒ��HandObject
	HandObject->SetChildActorClass(HandObjectClass);
}

void ASlAiPlayerCharacter::ChangeHandObjectDetect(bool IsOpen)
{
	//��ȡ������Ʒ
	ASlAiHandObject* HandObjectClass = Cast<ASlAiHandObject>(HandObject->GetChildActor());
	if (HandObjectClass)
		HandObjectClass->ChangeOverlayDetect(IsOpen);
	//�޸Ĺ���״̬
	IsAttack = IsOpen;
}

void ASlAiPlayerCharacter::RenderHandObject(bool IsRender)
{
	//���������Ʒû
	if (!HandObject->GetChildActor())
		return;

	HandObject->GetChildActor()->SetActorHiddenInGame(!IsRender);
}

void ASlAiPlayerCharacter::PlayerThrowObject(int ObjectID, int Num)
{
	if (GetWorld())
	{
		for (int i = 0; i < Num; ++i)
		{
			//���ɵ�����Դ
			ASlAiFlobObject* FlobObject = GetWorld()->SpawnActor<ASlAiFlobObject>(GetActorLocation() + FVector(0.f, 0.f, 40.f), FRotator::ZeroRotator);
			FlobObject->ThrowFlobObject(ObjectID, GetActorRotation().Yaw);
		}
	}
}

bool ASlAiPlayerCharacter::IsPackageFree(int ObjectID)
{
	return SlAiPackageManager::Get()->SearchFreeSpace(ObjectID);
}

void ASlAiPlayerCharacter::AddPackageObject(int ObjectID)
{
	SlAiPackageManager::Get()->AddObject(ObjectID);
}

void ASlAiPlayerCharacter::EatUpEvent()
{
	if (!SPController->SPState)
		return;

	if (SlAiPackageManager::Get()->EatUpEvent(SPController->SPState->CurrentShortcutIndex))
		SPController->SPState->PromoteHunger();
}


class UCameraComponent* ASlAiPlayerCharacter::GetCurrentCamera()
{
	switch (GameView)
	{
	case EGameViewMode::First:
		return FirstCamera;
	case EGameViewMode::Third:
		return ThirdCamera;
	default:
		return nullptr;
	}
}

bool ASlAiPlayerCharacter::IsPlayerDead()
{
	if (SPController->SPState)
		return SPController->SPState->IsPlayerDead();

	return false;
}

void ASlAiPlayerCharacter::AcceptDamage(int DamageValue)
{
	if (SPController->SPState)
		SPController->SPState->AcceptDamage(DamageValue);
}

void ASlAiPlayerCharacter::MoveForward(float Value)
{
	//�����������ס��ֱ�ӷ���
	if (IsInputLocked)
		return;

	if (Value != 0.f && Controller)
	{
		const FRotator Rotation = Controller->GetControlRotation();
		FVector Direction = FRotationMatrix(Rotation).GetScaledAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void ASlAiPlayerCharacter::MoveRight(float Value)
{
	//�����������ס��ֱ�ӷ���
	if (IsInputLocked)
		return;

	if (Value != 0)
	{
		const FQuat Rotation = GetActorQuat();
		FVector Direction = FQuatRotationMatrix(Rotation).GetScaledAxis(EAxis::Y);
		AddMovementInput(Direction, Value);
	}
}

void ASlAiPlayerCharacter::LookUpAtRate(float Value)
{
	//�����������ס��ֱ�ӷ���
	if (IsInputLocked)
		return;

	AddControllerPitchInput(Value * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void ASlAiPlayerCharacter::Turn(float Value)
{
	//�����������ס��ֱ�ӷ���
	if (IsInputLocked)
		return;

	AddControllerYawInput(Value);
}

void ASlAiPlayerCharacter::TurnAtRate(float Value)
{
	//�����������ס��ֱ�ӷ���
	if (IsInputLocked)
		return;

	AddControllerYawInput(Value * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void ASlAiPlayerCharacter::OnStartJump()
{
	//�����������ס��ֱ�ӷ���
	if (IsInputLocked)
		return;

	bPressedJump = true;
}

void ASlAiPlayerCharacter::OnStopJump()
{
	//�����������ס��ֱ�ӷ���
	if (IsInputLocked)
		return;

	bPressedJump = false;
	StopJumping();
}

void ASlAiPlayerCharacter::OnStartRun()
{
	//�����������ס��ֱ�ӷ���
	if (IsInputLocked)
		return;

	GetCharacterMovement()->MaxWalkSpeed = 375.f;
}

void ASlAiPlayerCharacter::OnStopRun()
{
	//�����������ס��ֱ�ӷ���
	if (IsInputLocked)
		return;

	GetCharacterMovement()->MaxWalkSpeed = 150.f;
}
