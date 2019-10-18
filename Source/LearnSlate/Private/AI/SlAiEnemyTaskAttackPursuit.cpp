// Fill out your copyright notice in the Description page of Project Settings.


#include "SlAiEnemyTaskAttackPursuit.h"
#include "SlAiEnemyController.h"
#include "SlAiEnemyCharacter.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "TimerManager.h"

EBTNodeResult::Type USlAiEnemyTaskAttackPursuit::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (!InitEnemyElement(OwnerComp))
		return EBTNodeResult::Failed;
	//播放攻击动画
	float AttackDuration = SECharacter->PlayAttackAction(EEnemyAttackType::EA_Pursuit);
	//设置速度
	SECharacter->SetMaxSpeed(600.f);
	//设置参数
	OwnerComp.GetBlackboardComponent()->SetValueAsBool(IsActionFinish.SelectedKeyName, false);
	//添加事件委托
	FTimerDelegate TimerDelegate = FTimerDelegate::CreateUObject(this, &USlAiEnemyTaskAttackPursuit::OnAnimationTimerDone);
	//注册事件管理
	SEController->GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDelegate, AttackDuration, false);
	return EBTNodeResult::Succeeded;
}

EBTNodeResult::Type USlAiEnemyTaskAttackPursuit::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	//如果初始化敌人参数不成功或者事件句柄没有激活，直接返回
	if (!InitEnemyElement(OwnerComp) || !TimerHandle.IsValid())
		return EBTNodeResult::Aborted;
	//卸载时间委托
	SEController->GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
	return EBTNodeResult::Aborted;
}

void USlAiEnemyTaskAttackPursuit::OnAnimationTimerDone()
{
	//设置动作完成
	if (SEController)
		SEController->ResetProcess(true);
	if (SECharacter)
		SECharacter->SetMaxSpeed(300.f);
}
