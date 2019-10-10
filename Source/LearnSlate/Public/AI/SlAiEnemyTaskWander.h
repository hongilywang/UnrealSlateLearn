// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AI/SlAiEnemyTaskBase.h"
#include "SlAiEnemyTaskWander.generated.h"

/**
 * 
 */
UCLASS()
class LEARNSLATE_API USlAiEnemyTaskWander : public USlAiEnemyTaskBase
{
	GENERATED_BODY()
	
public:
	//重写执行函数
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected:
	UPROPERTY(EditAnywhere, Category = "Blackboard")
		struct FBlackboardKeySelector Destination;

	//等待时长
	UPROPERTY(EditAnywhere, Category = "Blackboard")
		struct FBlackboardKeySelector WaitTime;
};
