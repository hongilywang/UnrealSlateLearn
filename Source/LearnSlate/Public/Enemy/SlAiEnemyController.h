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


protected:
	virtual void BeginPlay() override;
	

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
};
