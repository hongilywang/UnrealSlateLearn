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

	virtual void Possess(APawn* InPawn) override;

	virtual void UnPossess() override;

	virtual void Tick(float DeltaTime) override;

	//��ȡ��ҵ�λ��
	FVector GetPlayerLocation() const;

protected:
	virtual void BeginPlay() override;
	

private:
	//��ҽ�ɫ
	class ASlAiPlayerCharacter* SPCharacter;
	//���˽�ɫ
	class ASlAiEnemyCharacter* SECharacter;

	class UBlackboardComponent* BlackBoardComp;

	class UBehaviorTreeComponent* BehaviorComp;
};
