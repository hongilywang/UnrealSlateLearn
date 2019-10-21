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
	//进行委托绑定
	FTimerDelegate EPDisDele = FTimerDelegate::CreateUObject(this, &ASlAiEnemyController::UpdateStatePama);
	GetWorld()->GetTimerManager().SetTimer(EPDisHandle, EPDisDele, 0.3f, true);

	//初始化血量百分比
	HPRatio = 1.f;
	//设置状态计时器
	IsAllowHurt = false;
	HurtTimeCount = 0.f;
}

void ASlAiEnemyController::UpdateStatePama()
{
	//更新与玩家的距离序列
	if (EPDisList.Num() < 6)
	{
		EPDisList.Push(FVector::Distance(SECharacter->GetActorLocation(), GetPlayerLocation()));
	}
	else
	{
		EPDisList.RemoveAt(0);
		EPDisList.Push(FVector::Distance(SECharacter->GetActorLocation(), GetPlayerLocation()));
	}

	if (HurtTimeCount < 6.f)
	{
		HurtTimeCount += 0.3f;
	}
	else
	{
		HurtTimeCount = 0.f;
		IsAllowHurt = true;
	}

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

bool ASlAiEnemyController::IsPlayerAway()
{
	if (!IsLockPlayer || !SPCharacter || EPDisList.Num() < 6 || IsPlayerDead())
	{
		return false;
	}

	int BiggerNum = 0;
	float LastDis = -1.f;
	//只要偶3个比前面的大，判断远离
	for (TArray<float>::TIterator It(EPDisList); It; ++It)
	{
		if (*It < LastDis)
			BiggerNum += 1;
		LastDis = *It;
	}
	return BiggerNum > 3;
}

UObject* ASlAiEnemyController::GetPlayerPawn()
{
	return SPCharacter;
}

void ASlAiEnemyController::ResetProcess(bool IsFinish)
{
	//修改完成状态
	BlackBoardComp->SetValueAsBool("ProcessFinish", IsFinish);
}

void ASlAiEnemyController::UpdateDamageRatio(float HPRatioValue)
{
	//更新血量百分比
	HPRatio = HPRatioValue;
	//修改状态为受伤
	if (IsAllowHurt)
		BlackBoardComp->SetValueAsEnum("EnemyState", (uint8)EEnemyAIState::ES_Hurt);
	IsAllowHurt = false;
}

void ASlAiEnemyController::FinishStateHurt()
{
	//如果没有锁定玩家
	if (!IsLockPlayer)
		IsLockPlayer = true;

	if (HPRatio < 0.2f)
	{
		BlackBoardComp->SetValueAsEnum("EnemyState", (uint8)EEnemyAIState::ES_Escape);
	}
	else
	{
		FRandomStream Stream;
		Stream.GenerateNewSeed();
		int ActionRatio = Stream.RandRange(0, 10);
		//if (ActionRatio < 4)
		//	//进入防御状态
		//	BlackBoardComp->SetValueAsEnum("EnemyState", (uint8)EEnemyAIState::ES_Defence);
		//else
			//进入攻击状态
			BlackBoardComp->SetValueAsEnum("EnemyState", (uint8)EEnemyAIState::ES_Attack);
	}

}

