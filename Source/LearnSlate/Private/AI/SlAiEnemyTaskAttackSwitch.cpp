// Fill out your copyright notice in the Description page of Project Settings.


#include "SlAiEnemyTaskAttackSwitch.h"
#include "SlAiEnemyController.h"
#include "SlAiEnemyCharacter.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "SlAiTypes.h"

EBTNodeResult::Type USlAiEnemyTaskAttackSwitch::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (!InitEnemyElement(OwnerComp))
		return EBTNodeResult::Failed;

	//如果玩家死亡
	if (SEController->IsPlayerDead())
	{
		//告诉控制丢失玩家
		SEController->LoosePlayer();
		//修改状态为巡逻
		OwnerComp.GetBlackboardComponent()->SetValueAsEnum(EnemyState.SelectedKeyName, (uint8)EEnemyAIState::ES_Patrol);
		return EBTNodeResult::Failed;
	}

	//获取与玩家的距离
	float EPDistance = FVector::Distance(SECharacter->GetActorLocation(), SEController->GetPlayerLocation());
	if (EPDistance < 200.f)
	{
		//修改攻击状态为普攻
		OwnerComp.GetBlackboardComponent()->SetValueAsEnum(AttackType.SelectedKeyName, (uint8)EEnemyAttackType::EA_Normal);
		return EBTNodeResult::Succeeded;
	}

	//如果距离小于300并且判定到玩家的距离在远离
	if (EPDistance < 300.f && SEController->IsPlayerAway())
	{
		//修改状态为追逐攻击
		OwnerComp.GetBlackboardComponent()->SetValueAsEnum(AttackType.SelectedKeyName, (uint8)EEnemyAttackType::EA_Pursuit);
		return EBTNodeResult::Succeeded;
	}

	if (EPDistance > 200.f && EPDistance < 300.f)
	{
		//修改状态为冲刺
		OwnerComp.GetBlackboardComponent()->SetValueAsEnum(AttackType.SelectedKeyName, (uint8)EEnemyAttackType::EA_Dash);
		return EBTNodeResult::Succeeded;
	}

	if (EPDistance > 300.f)
	{
		//修改状态为追逐
		OwnerComp.GetBlackboardComponent()->SetValueAsEnum(EnemyState.SelectedKeyName, (uint8)EEnemyAIState::ES_Chase);
		return EBTNodeResult::Failed;
	}
	return EBTNodeResult::Failed;
}
