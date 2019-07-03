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
	//修改菜单音量
	void ResetMenuVolume(float MusicVol, float SoundVol);

private:
	//创建单例
	static TSharedRef<SlAiDataHandle> Create();

public:
	//语言
	ECultureTeam CurrentCulture;
	//音量
	float MusicVolume;
	//音效
	float SoundVolume;

private:
	static TSharedPtr<SlAiDataHandle> DataInstance;
};
