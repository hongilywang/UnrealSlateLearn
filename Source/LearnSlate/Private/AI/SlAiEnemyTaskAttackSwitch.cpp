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

	//����������
	if (SEController->IsPlayerDead())
	{
		//���߿��ƶ�ʧ���
		SEController->LoosePlayer();
		//�޸�״̬ΪѲ��
		OwnerComp.GetBlackboardComponent()->SetValueAsEnum(EnemyState.SelectedKeyName, (uint8)EEnemyAIState::ES_Patrol);
		return EBTNodeResult::Failed;
	}

	//��ȡ����ҵľ���
	float EPDistance = FVector::Distance(SECharacter->GetActorLocation(), SEController->GetPlayerLocation());
	if (EPDistance < 200.f)
	{
		//�޸Ĺ���״̬Ϊ�չ�
		OwnerComp.GetBlackboardComponent()->SetValueAsEnum(AttackType.SelectedKeyName, (uint8)EEnemyAttackType::EA_Normal);
		return EBTNodeResult::Succeeded;
	}

	//�������С��300�����ж�����ҵľ�����Զ��
	if (EPDistance < 300.f && SEController->IsPlayerAway())
	{
		//�޸�״̬Ϊ׷�𹥻�
		OwnerComp.GetBlackboardComponent()->SetValueAsEnum(AttackType.SelectedKeyName, (uint8)EEnemyAttackType::EA_Pursuit);
		return EBTNodeResult::Succeeded;
	}

	if (EPDistance > 200.f && EPDistance < 300.f)
	{
		//�޸�״̬Ϊ���
		OwnerComp.GetBlackboardComponent()->SetValueAsEnum(AttackType.SelectedKeyName, (uint8)EEnemyAttackType::EA_Dash);
		return EBTNodeResult::Succeeded;
	}

	if (EPDistance > 300.f)
	{
		//�޸�״̬Ϊ׷��
		OwnerComp.GetBlackboardComponent()->SetValueAsEnum(EnemyState.SelectedKeyName, (uint8)EEnemyAIState::ES_Chase);
		return EBTNodeResult::Failed;
	}
	return EBTNodeResult::Failed;
}
