// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Player/SlAiPlayerAnim.h"
#include "SlAiThirdPlayerAnim.generated.h"

/**
 * 
 */
UCLASS()
class LEARNSLATE_API USlAiThirdPlayerAnim : public USlAiPlayerAnim
{
	GENERATED_BODY()
	
public:
	USlAiThirdPlayerAnim();

public:

	//�Ƿ��ڿ���
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = PlayAnim)
		bool IsInAir;

	//����
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = PlayAnim)
		float Direction;
};
