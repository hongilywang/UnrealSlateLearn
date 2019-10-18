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
	//范围是5.0
	const float ChaseRadius = 5.f;
	//获取玩家到敌人直接的单位向量
	FVector SPToSE = SEController->GetPlayerLocation() - SECharacter->GetActorLocation();

	//获取距离
	float EPDistance = SPToSE.Size();

	//如果距离大于100
	if (EPDistance > 100)
	{
		SPToSE.Normalize();
		//探索起点是玩家位置减去与敌人直接距离的一点点
		const FVector ChaseOrigin = SEController->GetPlayerLocation() - 100.f * SPToSE;
		//保存随机的位置
		FVector DesLoc(0.f);
		//导航系统获取随机点
		UNavigationSystemV1::K2_GetRandomLocationInNavigableRadius(SEController, ChaseOrigin, DesLoc, ChaseRadius);
		//修改目的地
		OwnerComp.GetBlackboardComponent()->SetValueAsVector(Destination.SelectedKeyName, DesLoc);
	}
	else
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsVector(Destination.SelectedKeyName, SECharacter->GetActorLocation());
	}
	return EBTNodeResult::Succeeded;
}
