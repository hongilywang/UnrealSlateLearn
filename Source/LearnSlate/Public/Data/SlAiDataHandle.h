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
	//�޸Ĳ˵�����
	void ResetMenuVolume(float MusicVol, float SoundVol);

private:
	//��������
	static TSharedRef<SlAiDataHandle> Create();

	//����enum���ͻ�ȡ�ַ���
	template<typename TEnum>
	FString GetEnumValueAsString(const FString& Name, TEnum Value);

	//�����ַ�����ȡEnumֵ
	template<typename TEnum>
	TEnum GetEnumValueFromString(const FString& Name, FString Value);

public:
	//����
	ECultureTeam CurrentCulture;
	//����
	float MusicVolume;
	//��Ч
	float SoundVolume;

private:
	static TSharedPtr<SlAiDataHandle> DataInstance;
};
