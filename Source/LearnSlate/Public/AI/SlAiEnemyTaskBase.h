// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "SlAiEnemyTaskBase.generated.h"

/**
 * 
 */
UCLASS()
class LEARNSLATE_API USlAiEnemyTaskBase : public UBTTaskNode
{
	GENERATED_BODY()
	
protected:
	//��ʼ���������ͽ�ɫ�����ɹ�����false
	bool InitEnemyElement(UBehaviorTreeComponent& OwnerComp);

protected:
	class ASlAiEnemyController* SEController;
	class ASlAiEnemyCharacter* SECharacter;
};
