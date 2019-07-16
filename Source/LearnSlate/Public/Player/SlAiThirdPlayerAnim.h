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

	//是否在空中
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = PlayAnim)
		bool IsInAir;

	//方向
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = PlayAnim)
		float Direction;
};
