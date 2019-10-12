// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "SlAiEnemyController.generated.h"

/**
 * 
 */
UCLASS()
class LEARNSLATE_API ASlAiEnemyController : public AAIController
{
	GENERATED_BODY()
	
public:
	ASlAiEnemyController();

	virtual void OnPossess(APawn* InPawn) override;

	virtual void OnUnPossess() override;

	virtual void Tick(float DeltaTime) override;

	//获取玩家的位置
	FVector GetPlayerLocation() const;

	//玩家是否已经死亡
	bool IsPlayerDead();

	//看到了玩家，由character的OnSeePlayer调用
	void OnSeePlayer();

	//丢失玩家
	void LoosePlayer();


protected:
	virtual void BeginPlay() override;
	

public:
	//是否锁定了玩家
	bool IsLockPlayer;

private:
	//玩家角色
	class ASlAiPlayerCharacter* SPCharacter;
	//敌人角色
	class ASlAiEnemyCharacter* SECharacter;

	class UBlackboardComponent* BlackBoardComp;

	class UBehaviorTreeComponent* BehaviorComp;
};
