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

	virtual void Tick(float DeltaTime) override;

	//获取玩家的位置
	FVector GetPlayerLocation() const;

protected:
	virtual void BeginPlay() override;
	

private:
	class ASlAiPlayerCharacter* SPCharacter;
};
