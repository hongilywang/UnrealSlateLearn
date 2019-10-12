// Fill out your copyright notice in the Description page of Project Settings.


#include "SlAiEnemyTaskChaseSwitch.h"
#include "SlAiEnemyController.h"
#include "SlAiEnemyCharacter.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "SlAiTypes.h"

EBTNodeResult::Type USlAiEnemyTaskChaseSwitch::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	//�����ʼ�����˲������ɹ���ֱ�ӷ���ʧ��
	if (!InitEnemyElement(OwnerComp))
		return EBTNodeResult::Failed;
	//��ȡ����ҵľ���
	float EPDistance = FVector::Distance(SECharacter->GetActorLocation(), SEController->GetPlayerLocation());
	//�������С��300�� ״̬����Ϊ����������׷��
	if (EPDistance < 300.f)
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsEnum(EnemyState.SelectedKeyName, (uint8)EEnemyAIState::ES_Attack);
		return EBTNodeResult::Failed;
	}
	//����������1500������Ѳ��״̬,��������û���������
	if (EPDistance > 1500.f)
	{
		//���߿�������ʧ���
		SEController->LoosePlayer();
		//�޸�״̬ΪѲ��
		OwnerComp.GetBlackboardComponent()->SetValueAsEnum(EnemyState.SelectedKeyName, (uint8)EEnemyAIState::ES_Patrol);
		return EBTNodeResult::Failed;
	}

	return EBTNodeResult::Succeeded;
}
