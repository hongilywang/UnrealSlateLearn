// Fill out your copyright notice in the Description page of Project Settings.


#include "SlAiEnemyTaskDash.h"
#include "SlAiEnemyController.h"
#include "SlAiEnemyCharacter.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "TimerManager.h"

EBTNodeResult::Type USlAiEnemyTaskDash::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (!InitEnemyElement(OwnerComp))
		return EBTNodeResult::Failed;

	//播放突击动画
	float AttackDuration = SECharacter->PlayAttackAction(EEnemyAttackType::EA_Dash);
	//范围是0
	const float ChaseRadius = 5.f;
	//获取玩家到敌人直接的单位向量
	FVector SPToSE = SEController->GetPlayerLocation() - SECharacter->GetActorLocation();
	SPToSE.Normalize();
	//探索起点是玩家位置减去与敌人直接距离的一点点
	const FVector ChaseOrigin = SEController->GetPlayerLocation() - 20.f * SPToSE;
	//保存随机的位置
	FVector DesLoc(0.f);
	//导航系统获取随机点
	UNavigationSystemV1::K2_GetRandomLocationInNavigableRadius(SEController, ChaseOrigin, DesLoc, ChaseRadius);
	//角色速度
	float Speed = (FVector::Distance(SECharacter->GetActorLocation(), DesLoc)) / AttackDuration + 30.f;
	//修改速度
	SECharacter->SetMaxSpeed(Speed);
	//修改目的地
	OwnerComp.GetBlackboardComponent()->SetValueAsVector(Destination.SelectedKeyName, DesLoc);
	//设置参数
	OwnerComp.GetBlackboardComponent()->SetValueAsFloat(WaitTime.SelectedKeyName, AttackDuration);
	//添加事件委托
	FTimerDelegate TimerDelegate = FTimerDelegate::CreateUObject(this, &USlAiEnemyTaskDash::OnAnimationTimerDone);
	//注册到事件管理器
	SEController->GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDelegate, AttackDuration, false);
}

EBTNodeResult::Type USlAiEnemyTaskDash::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	//如果初始化敌人参加不成功或者事件句柄没有激活，直接返回
	if (!InitEnemyElement(OwnerComp) || !TimerHandle.IsValid())
		return EBTNodeResult::Aborted;

	//卸载时间委托
	SEController->GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
	return EBTNodeResult::Aborted;
}

void USlAiEnemyTaskDash::OnAnimationTimerDone()
{
	SECharacter->SetMaxSpeed(300.f);
}
