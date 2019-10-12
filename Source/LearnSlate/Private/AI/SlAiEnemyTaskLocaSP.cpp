// Fill out your copyright notice in the Description page of Project Settings.


#include "SlAiEnemyTaskLocaSP.h"
#include "SlAiEnemyController.h"
#include "SlAiEnemyCharacter.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

EBTNodeResult::Type USlAiEnemyTaskLocaSP::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	//�����ʼ�����˲������ɹ���ֱ�ӷ���ʧ��
	if (!InitEnemyElement(OwnerComp))
		return EBTNodeResult::Failed;

	//��Χ��20����
	const float ChaseRadius = 20.f;
	//��ȡ��ҵ�����֮��ĵ�λ����
	FVector SPToSE = SEController->GetPlayerLocation() - SECharacter->GetActorLocation();
	SPToSE.Normalize();
	//̽���������λ�ü�ȥ�����ֱ�Ӿ����һ��
	const FVector ChaseOrigin = SEController->GetPlayerLocation() - 100.f * SPToSE;
	//���������λ��
	FVector DesLoc(0.f);
	//ʹ�õ���ϵͳ��ȡ�����
	UNavigationSystemV1::K2_GetRandomReachablePointInRadius(SEController, ChaseOrigin, DesLoc, ChaseRadius);
	//�޸�Ŀ�ĵ�
	OwnerComp.GetBlackboardComponent()->SetValueAsVector(Destination.SelectedKeyName, DesLoc);
	SECharacter->SetMaxSpeed(300.f);
	return EBTNodeResult::Succeeded;
}
