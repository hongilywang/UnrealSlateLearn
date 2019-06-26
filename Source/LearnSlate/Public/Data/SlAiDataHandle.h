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

	//�޸���Ӣ��
	void ChangeLocalizationCulture(ECultureTeam Culture);

private:
	//��������
	static TSharedRef<SlAiDataHandle> Create();

public:
	ECultureTeam CurrentCulture;

private:
	static TSharedPtr<SlAiDataHandle> DataInstance;
};
