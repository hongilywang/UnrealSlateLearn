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
	//���Ź�������
	float AttackDuration = SECharacter->PlayAttackAction(EEnemyAttackType::EA_Pursuit);
	//�����ٶ�
	SECharacter->SetMaxSpeed(600.f);
	//���ò���
	OwnerComp.GetBlackboardComponent()->SetValueAsBool(IsActionFinish.SelectedKeyName, false);
	//����¼�ί��
	FTimerDelegate TimerDelegate = FTimerDelegate::CreateUObject(this, &USlAiEnemyTaskAttackPursuit::OnAnimationTimerDone);
	//ע���¼�����
	SEController->GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDelegate, AttackDuration, false);
	return EBTNodeResult::Succeeded;
}

EBTNodeResult::Type USlAiEnemyTaskAttackPursuit::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	//�����ʼ�����˲������ɹ������¼����û�м��ֱ�ӷ���
	if (!InitEnemyElement(OwnerComp) || !TimerHandle.IsValid())
		return EBTNodeResult::Aborted;
	//ж��ʱ��ί��
	SEController->GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
	return EBTNodeResult::Aborted;
}

void USlAiEnemyTaskAttackPursuit::OnAnimationTimerDone()
{
	//���ö������
	if (SEController)
		SEController->ResetProcess(true);
	if (SECharacter)
		SECharacter->SetMaxSpeed(300.f);
}
