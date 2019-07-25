// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "SlAiTypes.h"
#include "CoreMinimal.h"

class USoundCue;
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
	//游戏数据初始化
	void InitializeGameData();

private:
	//创建单例
	static TSharedRef<SlAiDataHandle> Create();

	//根据enum类型获取字符串
	template<typename TEnum>
	FString GetEnumValueAsString(const FString& Name, TEnum Value);

	//根据字符串获取Enum值
	template<typename TEnum>
	TEnum GetEnumValueFromString(const FString& Name, FString Value);

	//初始化存档数据
	void InitRecordData();
	//初始化Menu声音数据
	void InitializedMenuAudio();
	//初始化物品属性
	void InitializeObjectAttr();

public:
	//语言
	ECultureTeam CurrentCulture;
	//音量
	float MusicVolume;
	//音效
	float SoundVolume;

	//存档数据
	TArray<FString> RecordDataList;

	//存档名
	FString RecordName;

	//物品属性
	TMap<int, TSharedPtr<ObjectAttribute>> ObjectAttrMap;

private:
	static TSharedPtr<SlAiDataHandle> DataInstance;
	//保存Menu的声音
	TMap<FString, TArray<USoundCue*>> MenuAudioResource;
	//获取MenuStyle 里面存放有声音文件
	const struct FSlAiMenuStyle* MenuStyle;
};
