// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "SlAiTypes.h"
#include "CoreMinimal.h"

/**
 * 
 */
class LEARNSLATE_API SlAiDataHandle
{
public:
	SlAiDataHandle();
	static void Initialize();
	static TSharedPtr<SlAiDataHandle> Get();

	//修改中英文
	void ChangeLocalizationCulture(ECultureTeam Culture);

private:
	//创建单例
	static TSharedRef<SlAiDataHandle> Create();

public:
	ECultureTeam CurrentCulture;

private:
	static TSharedPtr<SlAiDataHandle> DataInstance;
};
