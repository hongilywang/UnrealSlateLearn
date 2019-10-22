// Fill out your copyright notice in the Description page of Project Settings.


#include "SlAiEnemyTaskAttackNormal.h"
#include "SlAiEnemyController.h"
#include "SlAiEnemyCharacter.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

EBTNodeResult::Type USlAiEnemyTaskAttackNormal::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (!InitEnemyElement(OwnerComp))
		return EBTNodeResult::Failed;

	//播放普通攻击动画
	float AttackDuration = SECharacter->PlayAttackAction(EEnemyAttackType::EA_Normal);
	//设置参数
	OwnerComp.GetBlackboardComponent()->SetValueAsFloat(WaitTime.SelectedKeyName, AttackDuration);
	//返回成功
	return EBTNodeResult::Succeeded;
}
