// Fill out your copyright notice in the Description page of Project Settings.


#include "SlAiEnemyTaskWander.h"
#include "SlAiEnemyCharacter.h"
#include "SlAiEnemyController.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"

EBTNodeResult::Type USlAiEnemyTaskWander::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (!InitEnemyElement(OwnerComp))
		return EBTNodeResult::Failed;

	//���ķ�Χ��1000
	const float WanderRadius = 1000.f;
	//���������Լ���λ��
	const FVector WanderOrigin = SECharacter->GetActorLocation();
	//���������λ��
	FVector DesLoc(0.f);
	//ʹ�õ���ϵͳ��ȡ�����
	UNavigationSystem::K2_GetRandomReachablePointInRadius(SEController, WanderOrigin, DesLoc, WanderRadius);
	//��ǰ����С��500�� �����ҵ�
	while (FVector::Distance(WanderOrigin, DesLoc) < 500.f)
	{
		UNavigationSystem::K2_GetRandomReachablePointInRadius(SEController, WanderOrigin, DesLoc, WanderRadius);
	}
	//�޸��ٶ�Ϊ100
	SECharacter->SetMaxSpeed(100.f);

	//�޸�Ŀ�ĵ�
	OwnerComp.GetBlackboardComponent()->SetValueAsVector(Destination.SelectedKeyName, DesLoc);
	//��ȡֹͣʱ��
	float TotalWaitTime = SECharacter->GetIdleWaitTime();
	//�޸ĵȴ�ʱ��
	OwnerComp.GetBlackboardComponent()->SetValueAsFloat(WaitTime.SelectedKeyName, TotalWaitTime);
	
	return EBTNodeResult::Succeeded;
}
