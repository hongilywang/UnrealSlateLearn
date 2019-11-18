// Fill out your copyright notice in the Description page of Project Settings.


#include "SlAiEnemyCharacter.h"

#include "SlAiEnemyController.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/BoxComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Perception/PawnSensingComponent.h"
#include "ConstructorHelpers.h"

#include "SlAiEnemyTool.h"
#include "WidgetComponent.h"
#include "SSlAiEnemyHPWidget.h"
#include "SlAiPlayerCharacter.h"
#include "SlAiHelper.h"
#include "SlAiEnemyAnim.h"
#include "SlAiDataHandle.h"
#include "SlAiFlobObject.h"

// Sets default values
ASlAiEnemyCharacter::ASlAiEnemyCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//����Ai������
	AIControllerClass = ASlAiEnemyController::StaticClass();

	//������ײ�������ļ�
	GetCapsuleComponent()->SetCollisionProfileName(FName("EnemyProfile"));
	GetCapsuleComponent()->SetGenerateOverlapEvents(true);

	//���ģ��
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> StaticEnemyMesh(TEXT("SkeletalMesh'/Game/Res/PolygonAdventure/Mannequin/Enemy/SkMesh/Enemy.Enemy'"));
	GetMesh()->SetSkeletalMesh(StaticEnemyMesh.Object);
	GetMesh()->SetCollisionObjectType(ECC_Pawn);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetMesh()->SetCollisionResponseToAllChannels(ECR_Ignore);
	GetMesh()->SetRelativeLocation(FVector(0.f, 0.f, -95.f));
	GetMesh()->SetRelativeRotation(FQuat::MakeFromEuler(FVector(0.f, 0.f, -90.f)));

	//��Ӷ�����ͼ
	static ConstructorHelpers::FClassFinder<UAnimInstance> StaticEnemyAnim(TEXT("AnimBlueprint'/Game/Blueprint/Enemy/Enemy_Animation.Enemy_Animation_C'"));
	GetMesh()->AnimClass = StaticEnemyAnim.Class;

	//ʵ�������
	WeaponSocket = CreateDefaultSubobject<UChildActorComponent>(TEXT("WeaponSocket"));
	SheildSocket = CreateDefaultSubobject<UChildActorComponent>(TEXT("SheildSocket"));

	//ʵ����Ѫ��
	HPBar = CreateDefaultSubobject<UWidgetComponent>(TEXT("HPBar"));
	//HPBar->AttachTo(RootComponent);
	FAttachmentTransformRules AttachmentRules(EAttachmentRule::KeepRelative, false);
	HPBar->AttachToComponent(RootComponent, AttachmentRules);

	//ʵ�������˸�֪���
	EnemySense = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("EnemySense"));

	//��������������Դ
	AnimDead_I = Cast<UAnimationAsset>(StaticLoadObject(UAnimationAsset::StaticClass(), nullptr, *FString("AnimSequence'/Game/Res/PolygonAdventure/Mannequin/Enemy/Animation/FightGroup/Enemy_Dead_I.Enemy_Dead_I'")));
	AnimDead_II = Cast<UAnimationAsset>(StaticLoadObject(UAnimationAsset::StaticClass(), nullptr, *FString("AnimSequence'/Game/Res/PolygonAdventure/Mannequin/Enemy/Animation/FightGroup/Enemy_Dead_II.Enemy_Dead_II'")));

	IsDestroyNextTick = false;
}

// Called when the game starts or when spawned
void ASlAiEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	//��ȡ����
	SEAnim = Cast<USlAiEnemyAnim>(GetMesh()->GetAnimInstance());
	SEController = Cast<ASlAiEnemyController>(GetController());

	//�󶨲��
	WeaponSocket->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName("RHSocket"));
	SheildSocket->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName("LHSocket"));

	//����������Ʒ
	WeaponSocket->SetChildActorClass(ASlAiEnemyTool::SpawnEnemyWeapon());
	SheildSocket->SetChildActorClass(ASlAiEnemyTool::SpawnEnemySheild());

	//����Ѫ��widget
	SAssignNew(HPBarWidget, SSlAiEnemyHPWidget);
	HPBar->SetSlateWidget(HPBarWidget);
	HPBar->SetRelativeLocation(FVector(0.f, 0.f, 100.f));
	HPBar->SetDrawSize(FVector2D(100.f, 10.f));
	//��ʼ��Ѫ��
	HP = 200.f;
	HPBarWidget->ChangeHP(HP / 200.f);

	//���˸�֪��������
	EnemySense->HearingThreshold = 0.f;
	EnemySense->LOSHearingThreshold = 0.f;
	EnemySense->SightRadius = 1000.f;
	EnemySense->SetPeripheralVisionAngle(55.f);
	EnemySense->bHearNoises = false;
	//�󶨿�����ҵķ���
	FScriptDelegate OnSeePlayerDele;
	OnSeePlayerDele.BindUFunction(this, "OnSeePlayer");
	EnemySense->OnSeePawn.Add(OnSeePlayerDele);

	//������ԴID
	ResourceIndex = 3;
}

void ASlAiEnemyCharacter::CreateFlobObject()
{
	TSharedPtr<ResourceAttribute> ResourceAttr = *SlAiDataHandle::Get()->ResourceAttrMap.Find(ResourceIndex);

	//��������
	for (TArray<TArray<int>>::TIterator It(ResourceAttr->FlobObjectInfo); It; ++It)
	{
		FRandomStream Stream;
		Stream.GenerateNewSeed();
		int Num = Stream.RandRange((*It)[1], (*It)[2]);

		if (GetWorld())
		{
			for (int i = 0; i < Num; ++i)
			{
				ASlAiFlobObject* FlobObject = GetWorld()->SpawnActor<ASlAiFlobObject>(GetActorLocation() + FVector(0.f, 0.f, 40.f), FRotator::ZeroRotator);
				FlobObject->CreateFlobObject((*It)[0]);
			}
		}
	}
}

// Called every frame
void ASlAiEnemyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsDestroyNextTick)
		DestroyEvent();
}

// Called to bind functionality to input
void ASlAiEnemyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ASlAiEnemyCharacter::UpdateHPBarRotation(FVector SPLocation)
{
	FVector StartPos(GetActorLocation().X, GetActorLocation().Y, 0);
	FVector TargetPos(SPLocation.X, SPLocation.Y, 0);
	HPBar->SetWorldRotation(FRotationMatrix::MakeFromX(TargetPos - StartPos).Rotator());
}

void ASlAiEnemyCharacter::SetMaxSpeed(float Speed)
{
	GetCharacterMovement()->MaxWalkSpeed = Speed;
}

float ASlAiEnemyCharacter::GetIdleWaitTime()
{
	if (!SEAnim)
		return 3.f;

	//���������
	FRandomStream Stream;
	Stream.GenerateNewSeed();
	int IdleType = Stream.RandRange(0, 2);
	float AnimLength = SEAnim->SetIdelType(IdleType);
	//�����������
	Stream.GenerateNewSeed();
	//�����������
	int AnimCount = Stream.RandRange(1, 4);
	//����ȫ������ʱ��
	return AnimLength * AnimCount;
}

float ASlAiEnemyCharacter::PlayAttackAction(EEnemyAttackType AttackType)
{
	if (!SEAnim)
		return 0.f;
	return SEAnim->PlayAttackAction(AttackType);
}

void ASlAiEnemyCharacter::AcceptDamage(int DamageValue)
{
	//�������������ֱ�ӷ���
	if (SEAnim && SEAnim->IsDefence)
		return;

	//Ѫ������
	HP = FMath::Clamp<float>(HP - DamageValue, 0.f, 500.f);
	HPBarWidget->ChangeHP(HP / 200.f);
	//���ѪֵС��0
	if (HP <= 0.f && !DeadHandle.IsValid())
	{
		//���߿���������
		SEController->EnemyDead();
		//ֹͣ���ж���
		SEAnim->StopAllAction();

		float DeadDuration = 0.f;
		FRandomStream Stream;
		Stream.GenerateNewSeed();
		int SelectIndex = Stream.RandRange(0, 1);
		if (SelectIndex == 0)
		{
			GetMesh()->PlayAnimation(AnimDead_I, false);
			DeadDuration = AnimDead_I->GetMaxCurrentTime() * 2;
		}
		else
		{
			GetMesh()->PlayAnimation(AnimDead_II, false);
			DeadDuration = AnimDead_II->GetMaxCurrentTime() * 2;
		}

		//���ɵ�����
		CreateFlobObject();

		//����¼�ί��
		FTimerDelegate TimerDelegate = FTimerDelegate::CreateUObject(this, &ASlAiEnemyCharacter::DestroyEvent);
		GetWorld()->GetTimerManager().SetTimer(DeadHandle, TimerDelegate, DeadDuration, false);
	}
	else
	{
		if (SEController)
			SEController->UpdateDamageRatio(HP / 200.f);
	}
}

float ASlAiEnemyCharacter::PlayHurtAction()
{
	if (!SEAnim)
		return 0.f;

	return SEAnim->PlayHurtAction();
}

void ASlAiEnemyCharacter::StartDefence()
{
	if (SEAnim)
		SEAnim->IsDefence = true;
}

void ASlAiEnemyCharacter::StopDefence()
{
	if (SEAnim)
		SEAnim->IsDefence = false;
}

void ASlAiEnemyCharacter::DestroyEvent()
{
	//ע��ʱ�亯��
	if (DeadHandle.IsValid())
		GetWorld()->GetTimerManager().ClearTimer(DeadHandle);
	//�����Լ�
	GetWorld()->DestroyActor(this);
}

FText ASlAiEnemyCharacter::GetInfoText() const
{
	TSharedPtr<ResourceAttribute> ResourceAttr = *SlAiDataHandle::Get()->ResourceAttrMap.Find(ResourceIndex);
	switch (SlAiDataHandle::Get()->CurrentCulture)
	{
	case ECultureTeam::EN:
		return ResourceAttr->EN;
	case ECultureTeam::ZH:
		return ResourceAttr->ZH;
	}
	return ResourceAttr->ZH;
}

void ASlAiEnemyCharacter::ChangeWeaponDetect(bool IsOpen)
{
	ASlAiEnemyTool* WeaponClass = Cast<ASlAiEnemyTool>(WeaponSocket->GetChildActor());
	if (WeaponClass)
		WeaponClass->ChangeOverlayDetect(IsOpen);
}

bool ASlAiEnemyCharacter::IsLockPlayer()
{
	if (SEController)
		return SEController->IsLockPlayer;
	return false;
}

void ASlAiEnemyCharacter::LoadHP(float HPValue)
{
	HP = HPValue;
	//����UI
	HPBarWidget->ChangeHP(HP / 200.f);
}

void ASlAiEnemyCharacter::OnSeePlayer(APawn* PlayerChar)
{
	if (Cast<ASlAiPlayerCharacter>(PlayerChar))
	{
		SlAiHelper::Debug(FString("See Player"), 1.0f);
		//���߿������ҿ��������
		if (SEController)
			SEController->OnSeePlayer();
	}
}

