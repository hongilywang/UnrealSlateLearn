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

	//设置人物碰撞属性为playerProfile，下面的mesh模型的碰撞就可以设置无无碰撞
	GetCapsuleComponent()->SetCollisionProfileName(FName("PlayerProfile"));

	//添加第一人称骨骼模型
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> StaticMeshFirst(TEXT("SkeletalMesh'/Game/Res/PolygonAdventure/Mannequin/FirstPlayer/SkMesh/FirstPlayer.FirstPlayer'"));
	MeshFirst = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MeshFirst"));
	MeshFirst->SetSkeletalMesh(StaticMeshFirst.Object);
	MeshFirst->SetupAttachment((USceneComponent*)GetCapsuleComponent());
	MeshFirst->bOnlyOwnerSee = true;
	MeshFirst->bCastDynamicShadow = false;
	MeshFirst->bReceivesDecals = false;
	//更新频率衰落
	MeshFirst->VisibilityBasedAnimTickOption = EVisibilityBasedAnimTickOption::OnlyTickPoseWhenRendered;
	MeshFirst->PrimaryComponentTick.TickGroup = TG_PrePhysics;
	//设置碰撞属性
	MeshFirst->SetCollisionObjectType(ECC_Pawn);
	MeshFirst->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	MeshFirst->SetCollisionResponseToAllChannels(ECR_Ignore);
	//设置位移
	MeshFirst->SetRelativeLocation(FVector(0.f, 0.f, -90.f));
	MeshFirst->SetRelativeRotation(FQuat::MakeFromEuler(FVector(0.0f, 0.f, -90.f)));

	//获取第一人称的动作蓝图
	//如果是动画蓝图，Ctrl C，Ctrl V后的路径需要加上_C
	static ConstructorHelpers::FClassFinder<UAnimInstance> StaticAnimFirst(TEXT("AnimBlueprint'/Game/Blueprint/Player/FirstPlayer_Animation.FirstPlayer_Animation_C'"));
	MeshFirst->AnimClass = StaticAnimFirst.Class;

	//给默认Mesh添加骨骼模型
	//模型的路径获取在unity编辑器里面，找到对应的模型文件，然后Ctrl+C复制就可以
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> StaticMeshThird(TEXT("SkeletalMesh'/Game/Res/PolygonAdventure/Mannequin/Player/SkMesh/Player.Player'"));
	GetMesh()->SetSkeletalMesh(StaticMeshThird.Object);
	GetMesh()->bOnlyOwnerSee = true;
	GetMesh()->bReceivesDecals = false;
	GetMesh()->SetCollisionObjectType(ECC_Pawn);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetMesh()->SetCollisionResponseToAllChannels(ECR_Ignore);
	GetMesh()->SetRelativeLocation(FVector(0.f, 0.f, -90.f));
	GetMesh()->SetRelativeRotation(FQuat::MakeFromEuler(FVector(0.0f, 0.f, -90.f)));

	//获取第三人称的动作蓝图
	//如果是动画蓝图，Ctrl C，Ctrl V后的路径需要加上_C
	static ConstructorHelpers::FClassFinder<UAnimInstance> StaticAnimThird(TEXT("AnimBlueprint'/Game/Blueprint/Player/ThirdPlayer_Animation.ThirdPlayer_Animation_C'"));
	GetMesh()->AnimClass = StaticAnimThird.Class;

	//摄像机手臂
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	//设置距离
	CameraBoom->TargetArmLength = 300.f;
	//设置偏移
	CameraBoom->TargetOffset = FVector(0.f, 0.f, 60.f);
	//绑定controller的旋转
	CameraBoom->bUsePawnControlRotation = true;

	//初始化第三人称摄像机
	ThirdCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("ThirdCamera"));
	ThirdCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	//设置ThirdCamera不跟随控制器的旋转
	ThirdCamera->bUsePawnControlRotation = false;

	//初始化第一人摄像机
	FirstCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstCamera"));
	FirstCamera->SetupAttachment((USceneComponent*)GetCapsuleComponent());
	//设置跟随controller的旋转
	FirstCamera->bUsePawnControlRotation = true;
	//设置位置
	FirstCamera->AddLocalOffset(FVector(0.f, 0.f, 60.f));

	//默认第三人称
	FirstCamera->SetActive(false);
	ThirdCamera->SetActive(true);
	//不显示第一人称模型
	GetMesh()->SetOwnerNoSee(false);
	MeshFirst->SetOwnerNoSee(true);

	//实例化手上物品
	HandObject = CreateDefaultSubobject<UChildActorComponent>(TEXT("HandObject"));

	//获取死亡动作资源
	AnimDead = Cast<UAnimationAsset>(StaticLoadObject(UAnimationAsset::StaticClass(), nullptr, *FString("AnimSequence'/Game/Res/PolygonAdventure/Mannequin/Player/Animation/Player_Death.Player_Death'")));

	//初始化参数
	BaseLookUpRate = 45.f;
	BaseTurnRate = 45.f;
	//设置初始速度为150
	GetCharacterMovement()->MaxWalkSpeed = 150.f;
	//初始化第三人视角
	GameView = EGameViewMode::Third;
	//上半身动作初始化为无动作
	UpperType = EUpperBody::None;
	//一开始允许切换视角
	IsAllowSwitch = true;

	IsInputLocked = false;
	//初始化没有攻击
	IsAttack = false;
}

// Called when the game starts or when spawned
void ASlAiPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	SPController = Cast<ASlAiPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));

	//把手持物品组件绑定到第三人模型右手插槽上
	HandObject->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName("RHSocket"));

	//添加Actor到HandeObject
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
		//修改handobject绑定位置
		HandObject->AttachToComponent(MeshFirst, FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName("RHSocket"));
		break;
	case EGameViewMode::Third:
		FirstCamera->SetActive(false);
		ThirdCamera->SetActive(true);
		MeshFirst->SetOwnerNoSee(true);
		GetMesh()->SetOwnerNoSee(false);
		//修改handobject绑定位置
		HandObject->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName("RHSocket"));
		break;
	}
}

void ASlAiPlayerCharacter::ChangeHandObject(TSubclassOf<class AActor> HandObjectClass)
{
	//设置物品到HandObject
	HandObject->SetChildActorClass(HandObjectClass);
}

void ASlAiPlayerCharacter::ChangeHandObjectDetect(bool IsOpen)
{
	//获取手上物品
	ASlAiHandObject* HandObjectClass = Cast<ASlAiHandObject>(HandObject->GetChildActor());
	if (HandObjectClass)
		HandObjectClass->ChangeOverlayDetect(IsOpen);
	//修改攻击状态
	IsAttack = IsOpen;
}

void ASlAiPlayerCharacter::RenderHandObject(bool IsRender)
{
	//如果手上物品没
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
			//生成掉落资源
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

float ASlAiPlayerCharacter::PlayDeadAnim()
{
	GetMesh()->PlayAnimation(AnimDead, false);
	return AnimDead->GetMaxCurrentTime();
}

void ASlAiPlayerCharacter::MoveForward(float Value)
{
	//如果操作被锁住，直接返回
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
	//如果操作被锁住，直接返回
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
	//如果操作被锁住，直接返回
	if (IsInputLocked)
		return;

	AddControllerPitchInput(Value * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void ASlAiPlayerCharacter::Turn(float Value)
{
	//如果操作被锁住，直接返回
	if (IsInputLocked)
		return;

	AddControllerYawInput(Value);
}

void ASlAiPlayerCharacter::TurnAtRate(float Value)
{
	//如果操作被锁住，直接返回
	if (IsInputLocked)
		return;

	AddControllerYawInput(Value * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void ASlAiPlayerCharacter::OnStartJump()
{
	//如果操作被锁住，直接返回
	if (IsInputLocked)
		return;

	bPressedJump = true;
}

void ASlAiPlayerCharacter::OnStopJump()
{
	//如果操作被锁住，直接返回
	if (IsInputLocked)
		return;

	bPressedJump = false;
	StopJumping();
}

void ASlAiPlayerCharacter::OnStartRun()
{
	//如果操作被锁住，直接返回
	if (IsInputLocked)
		return;

	GetCharacterMovement()->MaxWalkSpeed = 375.f;
}

void ASlAiPlayerCharacter::OnStopRun()
{
	//如果操作被锁住，直接返回
	if (IsInputLocked)
		return;

	GetCharacterMovement()->MaxWalkSpeed = 150.f;
}
