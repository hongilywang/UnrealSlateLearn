// Fill out your copyright notice in the Description page of Project Settings.


#include "SlAiEnemyTaskAttackFollow.h"
#include "SlAiEnemyController.h"
#include "SlAiEnemyCharacter.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

EBTNodeResult::Type USlAiEnemyTaskAttackFollow::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (!InitEnemyElement(OwnerComp))
		return EBTNodeResult::Failed;
	//��Χ��5.0
	const float ChaseRadius = 5.f;
	//��ȡ��ҵ�����ֱ�ӵĵ�λ����
	FVector SPToSE = SEController->GetPlayerLocation() - SECharacter->GetActorLocation();

	//��ȡ����
	float EPDistance = SPToSE.Size();

	//����������100
	if (EPDistance > 100)
	{
		SPToSE.Normalize();
		//̽����������λ�ü�ȥ�����ֱ�Ӿ����һ���
		const FVector ChaseOrigin = SEController->GetPlayerLocation() - 100.f * SPToSE;
		//���������λ��
		FVector DesLoc(0.f);
		//����ϵͳ��ȡ�����
		UNavigationSystemV1::K2_GetRandomLocationInNavigableRadius(SEController, ChaseOrigin, DesLoc, ChaseRadius);
		//�޸�Ŀ�ĵ�
		OwnerComp.GetBlackboardComponent()->SetValueAsVector(Destination.SelectedKeyName, DesLoc);
	}
	else
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsVector(Destination.SelectedKeyName, SECharacter->GetActorLocation());
	}
	return EBTNodeResult::Succeeded;
}
