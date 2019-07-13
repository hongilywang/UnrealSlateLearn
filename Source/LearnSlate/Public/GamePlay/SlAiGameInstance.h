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
	//module���غ�Instance��Init�ӿڻ���������,����������ӿ��г�ʼ��һЩ����
	//virtual void Init() override;

	UPROPERTY(VisibleAnywhere, Category = "SlAi")
		FString GameName;
};
