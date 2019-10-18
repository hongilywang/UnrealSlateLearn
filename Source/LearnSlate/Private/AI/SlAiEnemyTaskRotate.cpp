// Fill out your copyright notice in the Description page of Project Settings.


#include "SlAiEnemyTaskRotate.h"
#include "SlAiEnemyController.h"
#include "SlAiEnemyCharacter.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

EBTNodeResult::Type USlAiEnemyTaskRotate::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (!InitEnemyElement(OwnerComp))
		return EBTNodeResult::Failed;
	//��ȡ��ҵ����˵���������ȥY����
	FVector SEToSP = FVector(SEController->GetPlayerLocation().X, SEController->GetPlayerLocation().Y, 0.f) - (SECharacter->GetActorLocation().X, SECharacter->GetActorLocation().Y, 0.f);
	SEToSP.Normalize();
	//���ߵ����µĳ���
	SECharacter->UpdateRotation(FRotationMatrix::MakeFromX(SEToSP).Rotator());
	return EBTNodeResult::Succeeded;
}
