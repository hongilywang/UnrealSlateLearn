// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AI/SlAiEnemyTaskBase.h"
#include "SlAiEnemyTaskChaseSwitch.generated.h"

/**
 * 
 */
UCLASS()
class LEARNSLATE_API USlAiEnemyTaskChaseSwitch : public USlAiEnemyTaskBase
{
	GENERATED_BODY()
	
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected:

	UPROPERTY(EditAnywhere, Category = "Blackboard")
		struct FBlackboardKeySelector EnemyState;
};
