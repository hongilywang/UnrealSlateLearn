// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "SlAiGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class LEARNSLATE_API USlAiGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	//module加载后，Instance的Init接口会最先运行,可以在这个接口中初始化一些数据
	//virtual void Init() override;

	UPROPERTY(VisibleAnywhere, Category = "SlAi")
		FString GameName;
};
