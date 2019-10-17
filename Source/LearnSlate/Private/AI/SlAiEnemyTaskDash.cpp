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

	//����ͻ������
	float AttackDuration = SECharacter->PlayAttackAction(EEnemyAttackType::EA_Dash);
	//��Χ��0
	const float ChaseRadius = 5.f;
	//��ȡ��ҵ�����ֱ�ӵĵ�λ����
	FVector SPToSE = SEController->GetPlayerLocation() - SECharacter->GetActorLocation();
	SPToSE.Normalize();
	//̽����������λ�ü�ȥ�����ֱ�Ӿ����һ���
	const FVector ChaseOrigin = SEController->GetPlayerLocation() - 20.f * SPToSE;
	//���������λ��
	FVector DesLoc(0.f);
	//����ϵͳ��ȡ�����
	UNavigationSystemV1::K2_GetRandomLocationInNavigableRadius(SEController, ChaseOrigin, DesLoc, ChaseRadius);
	//��ɫ�ٶ�
	float Speed = (FVector::Distance(SECharacter->GetActorLocation(), DesLoc)) / AttackDuration + 30.f;
	//�޸��ٶ�
	SECharacter->SetMaxSpeed(Speed);
	//�޸�Ŀ�ĵ�
	OwnerComp.GetBlackboardComponent()->SetValueAsVector(Destination.SelectedKeyName, DesLoc);
	//���ò���
	OwnerComp.GetBlackboardComponent()->SetValueAsFloat(WaitTime.SelectedKeyName, AttackDuration);
	//����¼�ί��
	FTimerDelegate TimerDelegate = FTimerDelegate::CreateUObject(this, &USlAiEnemyTaskDash::OnAnimationTimerDone);
	//ע�ᵽ�¼�������
	SEController->GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDelegate, AttackDuration, false);
}

EBTNodeResult::Type USlAiEnemyTaskDash::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	//�����ʼ�����˲μӲ��ɹ������¼����û�м��ֱ�ӷ���
	if (!InitEnemyElement(OwnerComp) || !TimerHandle.IsValid())
		return EBTNodeResult::Aborted;

	//ж��ʱ��ί��
	SEController->GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
	return EBTNodeResult::Aborted;
}

void USlAiEnemyTaskDash::OnAnimationTimerDone()
{
	SECharacter->SetMaxSpeed(300.f);
}
