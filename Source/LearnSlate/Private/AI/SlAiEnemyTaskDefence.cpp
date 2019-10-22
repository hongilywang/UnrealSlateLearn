// Fill out your copyright notice in the Description page of Project Settings.


#include "SlAiEnemyTaskDefence.h"
#include "SlAiEnemyController.h"
#include "SlAiEnemyCharacter.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "TimerManager.h"

EBTNodeResult::Type USlAiEnemyTaskDefence::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (!InitEnemyElement(OwnerComp))
		return EBTNodeResult::Failed;
	//播放防御动画
	SECharacter->StartDefence();
	//设置没有结束状态
	OwnerComp.GetBlackboardComponent()->SetValueAsBool(IsDefenceFinish.SelectedKeyName, false);
	OwnerComp.GetBlackboardComponent()->SetValueAsObject(PlayerPawn.SelectedKeyName, SEController->GetPlayerPawn());
	//添加事件委托
	FTimerDelegate TimerDelegate = FTimerDelegate::CreateUObject(this, &USlAiEnemyTaskDefence::OnAnimationTimerDone);
	//注册事件到管理
	SEController->GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDelegate, 2.f, true);
	return EBTNodeResult::Succeeded;
}

EBTNodeResult::Type USlAiEnemyTaskDefence::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	//如果初始化敌人参加不成功或者事件句柄没有激活，直接返回
	if (!InitEnemyElement(OwnerComp) || !TimerHandle.IsValid())
		return EBTNodeResult::Aborted;

	//卸载时间委托
	SEController->GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
	return EBTNodeResult::Aborted;
}

void USlAiEnemyTaskDefence::OnAnimationTimerDone()
{
	if (SEController)
		SEController->FinishStateDefence();
}
