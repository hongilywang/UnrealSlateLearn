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

	//根据enum类型获取字符串
	template<typename TEnum>
	FString GetEnumValueAsString(const FString& Name, TEnum Value);

	//根据字符串获取Enum值
	template<typename TEnum>
	TEnum GetEnumValueFromString(const FString& Name, FString Value);

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
