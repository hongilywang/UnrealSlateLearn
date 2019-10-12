// Fill out your copyright notice in the Description page of Project Settings.


#include "SlAiEnemyController.h"
#include "Kismet/GameplayStatics.h"
#include "SlAiPlayerCharacter.h"
#include "SlAiEnemyCharacter.h"
#include "Camera/CameraComponent.h"

#include "SlAiEnemyBlackboard.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "TimerManager.h"

ASlAiEnemyController::ASlAiEnemyController()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ASlAiEnemyController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	//获取角色
	SECharacter = Cast<ASlAiEnemyCharacter>(InPawn);

	//获取行为树资源
	UBehaviorTree* StaticBehaviorTreeObject = LoadObject<UBehaviorTree>(nullptr, TEXT("BehaviorTree'/Game/Blueprint/Enemy/EnemyBehaviorTree.EnemyBehaviorTree'"));

	UBehaviorTree* BehaviorTreeObject = DuplicateObject<UBehaviorTree>(StaticBehaviorTreeObject, nullptr);

	//判断定资源是否存在
	if (!BehaviorTreeObject)
		return;

	BehaviorTreeObject->BlackboardAsset = DuplicateObject<USlAiEnemyBlackboard>((USlAiEnemyBlackboard*)StaticBehaviorTreeObject->BlackboardAsset, nullptr);

	BlackBoardComp = Blackboard;

	bool IsSuccess = true;

	//绑定资源 参考这个接口->RunBehaviorTree()
	if (BehaviorTreeObject->BlackboardAsset && (Blackboard == nullptr || Blackboard->IsCompatibleWith(BehaviorTreeObject->BlackboardAsset) == false))
	{
		IsSuccess = UseBlackboard(BehaviorTreeObject->BlackboardAsset, BlackBoardComp);
	}

	if (IsSuccess)
	{
		BehaviorComp = Cast<UBehaviorTreeComponent>(BrainComponent);
		if (!BehaviorComp)
		{
			BehaviorComp = NewObject<UBehaviorTreeComponent>(this, TEXT("BehaviorComp"));
			BehaviorComp->RegisterComponent();
		}
		BrainComponent = BehaviorComp;
		check(BehaviorComp != nullptr);
		BehaviorComp->StartTree(*BehaviorTreeObject, EBTExecutionMode::Looped);

		//设置预状态为巡逻
		BlackBoardComp->SetValueAsEnum("EnemyState", (uint8)EEnemyAIState::ES_Patrol);
		//也可以通过这种方式设置
		/*int32 EnemyStateIndex = BlackBoardComp->GetKeyID("EnemyState");
		BlackBoardComp->SetValue<UBlackboardKeyType_Enum>(EnemyStateIndex, (UBlackboardKeyType_Enum::FDataType)EEnemyAIState::ES_Patrol);*/

		//修改敌人的初始移动速度100
		SECharacter->SetMaxSpeed(100.f);
	}
}

void ASlAiEnemyController::OnUnPossess()
{
	Super::OnUnPossess();

	//停止行为树
	if (BehaviorComp)
		BehaviorComp->StopTree();
}

void ASlAiEnemyController::BeginPlay()
{
	//调用父类函数
	Super::BeginPlay();

	//
	SPCharacter = Cast<ASlAiPlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (!SECharacter)
		SECharacter = Cast<ASlAiEnemyCharacter>(GetPawn());
	//初始化，未锁定玩家
	IsLockPlayer = false;
}

void ASlAiEnemyController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//每帧更新血条的位置朝向
	if (SECharacter && SPCharacter && SPCharacter->GetCurrentCamera())
		SECharacter->UpdateHPBarRotation(SPCharacter->GetCurrentCamera()->GetComponentLocation());
}

FVector ASlAiEnemyController::GetPlayerLocation() const
{
	//如果玩家指针存在，返回玩家位置
	if (SPCharacter)
		return SPCharacter->GetActorLocation();

	return FVector::ZeroVector;
}

bool ASlAiEnemyController::IsPlayerDead()
{
	if (SPCharacter)
		return SPCharacter->IsPlayerDead();
	return false;
}

void ASlAiEnemyController::OnSeePlayer()
{
	//如果已经锁定了玩家或者玩家死亡，不再执行下面的函数
	if (IsLockPlayer || IsPlayerDead())
		return;

	//设置锁定玩家
	IsLockPlayer = true;
	//修改预状态为追逐
	BlackBoardComp->SetValueAsEnum("EnemyState", (uint8)EEnemyAIState::ES_Chase);
	SECharacter->SetMaxSpeed(300.f);
}

void ASlAiEnemyController::LoosePlayer()
{
	IsLockPlayer = false;
}

