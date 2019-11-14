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

	//�޸���Ӣ��
	void ChangeLocalizationCulture(ECultureTeam Culture);
	//�޸Ĳ˵�����
	void ResetMenuVolume(float MusicVol, float SoundVol);
	//�޸���Ϸ����
	void ResetGameVolume(float MusicVol, float SoundVol);
	//��Ϸ���ݳ�ʼ��
	void InitializeGameData();

private:
	//��������
	static TSharedRef<SlAiDataHandle> Create();

	//����enum���ͻ�ȡ�ַ���
	template<typename TEnum>
	FString GetEnumValueAsString(const FString& Name, TEnum Value);

	//�����ַ�����ȡEnumֵ
	template<typename TEnum>
	TEnum GetEnumValueFromString(const FString& Name, FString Value);

	//��ʼ���浵����
	void InitRecordData();
	//��ʼ��Menu��������
	void InitializedMenuAudio();
	//��ʼ����Ʒ����
	void InitializeObjectAttr();
	//��ʼ����Դ����
	void InitializeResourceAttr();
	//��ʼ���ϳɱ�
	void InitCompoundTableMap();
	//��ʼ��Game��������
	void InitializeGameAudio();
public:
	//����
	ECultureTeam CurrentCulture;
	//����
	float MusicVolume;
	//��Ч
	float SoundVolume;

	//�浵����
	TArray<FString> RecordDataList;

	//�浵��
	FString RecordName;

	//��Ʒ����
	TMap<int, TSharedPtr<ObjectAttribute>> ObjectAttrMap;
	//��Ʒ��ͼ��Դ����
	TArray<const FSlateBrush*> ObjectBrushList;
	//��Դ����
	TMap<int, TSharedPtr<ResourceAttribute>> ResourceAttrMap;
	//�ϳɱ�ͼ
	TArray<TSharedPtr<CompoundTable>> CompoundTableMap;

private:
	static TSharedPtr<SlAiDataHandle> DataInstance;
	//����Menu������
	TMap<FString, TArray<USoundCue*>> MenuAudioResource;
	//��ȡMenuStyle �������������ļ�
	const struct FSlAiMenuStyle* MenuStyle;
	//��ȡMenuStyle �������������ļ�
	const struct FSlAiGameStyle* GameStyle;
	//�������
	class USoundMix* SlAiSoundMix;
	class USoundClass* SlAiMusicClass;
	class USoundClass* SlAiSoundClass;
	class FAudioDevice* AudioDevice;
};
