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

	//判断玩家是否在远离
	bool IsPlayerAway();

	//获取玩家指针
	UObject* GetPlayerPawn();

	void ResetProcess(bool IsFinish);

	//接受伤害，传入剩余生命值
	void UpdateDamageRatio(float HPRatioValue);

	//完成伤害状态
	void FinishStateHurt();

	//完成防御状态
	void FinishStateDefence();

protected:
	virtual void BeginPlay() override;
	
private:
	//更新状态序列
	void UpdateStatePama();

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

	//保存与玩家的距离序列
	TArray<float> EPDisList;

	//时间委托句柄
	FTimerHandle EPDisHandle;

	//血值百分比
	float HPRatio;

	bool IsAllowHurt;
	float HurtTimeCount;
};
