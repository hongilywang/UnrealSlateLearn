// Fill out your copyright notice in the Description page of Project Settings.


#include "SlAiEnemyTaskEscapeSwitch.h"
#include "SlAiEnemyController.h"
#include "SlAiEnemyCharacter.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

EBTNodeResult::Type USlAiEnemyTaskEscapeSwitch::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (!InitEnemyElement(OwnerComp))
		return EBTNodeResult::Failed;

	//»ñÈ¡¾àÀë
	float EPDistance = FVector::Distance(SECharacter->GetActorLocation(), SEController->GetPlayerLocation());

	if (EPDistance > 1500.f)
	{
		SEController->LoosePlayer();
		//ÐÞ¸Ä×´Ì¬ÎªÑ²Âß
		OwnerComp.GetBlackboardComponent()->SetValueAsEnum(EnemyState.SelectedKeyName, (uint8)EEnemyAIState::ES_Patrol);
	}
	return EBTNodeResult::Succeeded;
}
