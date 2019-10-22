// Fill out your copyright notice in the Description page of Project Settings.


#include "SlAiEnemyTaskLocaESC.h"
#include "SlAiEnemyController.h"
#include "SlAiEnemyCharacter.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

EBTNodeResult::Type USlAiEnemyTaskLocaESC::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (!InitEnemyElement(OwnerComp))
		return EBTNodeResult::Failed;

	//范围2000
	const float ChaseRadius = 2000.f;
	//获取敌人到玩家之间的向量
	FVector SPToSE = SECharacter->GetActorLocation() - SEController->GetPlayerLocation();
	//探索起点敌人位置
	const FVector ChaseOrigin = SECharacter->GetActorLocation();
	//保存随机位置
	FVector DesLoc(0.f);
	UNavigationSystemV1::K2_GetRandomReachablePointInRadius(SEController, ChaseOrigin, DesLoc, ChaseRadius);

	//判断逃跑的方向
	while (FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(SPToSE, DesLoc - SECharacter->GetActorLocation()))) > 90.f)
	{
		UNavigationSystemV1::K2_GetRandomReachablePointInRadius(SEController, ChaseOrigin, DesLoc, ChaseRadius);
	} 

	SECharacter->SetMaxSpeed(300.f);
	OwnerComp.GetBlackboardComponent()->SetValueAsVector(Destination.SelectedKeyName, DesLoc);
	return EBTNodeResult::Succeeded;
}
