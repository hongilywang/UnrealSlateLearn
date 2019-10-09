// Fill out your copyright notice in the Description page of Project Settings.


#include "SlAiEnemyTaskBase.h"
#include "SlAiEnemyController.h"
#include "SlAiEnemyCharacter.h"

bool USlAiEnemyTaskBase::InitEnemyElement(UBehaviorTreeComponent& OwnerComp)
{
	//判断是否已经初始化
	if (SECharacter && SEController)
		return true;

	SEController = Cast<ASlAiEnemyController>(OwnerComp.GetAIOwner());
	if (SEController)
	{
		SECharacter = Cast<ASlAiEnemyCharacter>(SEController->GetPawn());
		if (SECharacter)
			return true;
	}
	return false;
}
