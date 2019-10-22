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

	//��ȡ��ҵ�λ��
	FVector GetPlayerLocation() const;

	//����Ƿ��Ѿ�����
	bool IsPlayerDead();

	//��������ң���character��OnSeePlayer����
	void OnSeePlayer();

	//��ʧ���
	void LoosePlayer();

	//�ж�����Ƿ���Զ��
	bool IsPlayerAway();

	//��ȡ���ָ��
	UObject* GetPlayerPawn();

	void ResetProcess(bool IsFinish);

	//�����˺�������ʣ������ֵ
	void UpdateDamageRatio(float HPRatioValue);

	//����˺�״̬
	void FinishStateHurt();

	//��ɷ���״̬
	void FinishStateDefence();

protected:
	virtual void BeginPlay() override;
	
private:
	//����״̬����
	void UpdateStatePama();

public:
	//�Ƿ����������
	bool IsLockPlayer;

private:
	//��ҽ�ɫ
	class ASlAiPlayerCharacter* SPCharacter;
	//���˽�ɫ
	class ASlAiEnemyCharacter* SECharacter;

	class UBlackboardComponent* BlackBoardComp;

	class UBehaviorTreeComponent* BehaviorComp;

	//��������ҵľ�������
	TArray<float> EPDisList;

	//ʱ��ί�о��
	FTimerHandle EPDisHandle;

	//Ѫֵ�ٷֱ�
	float HPRatio;

	bool IsAllowHurt;
	float HurtTimeCount;
};
