// Fill out your copyright notice in the Description page of Project Settings.


#include "SlAiDataHandle.h"
#include "Internationalization.h"
#include "SlAiSingleton.h"
#include "SlAiJsonHandle.h"
#include "SlAiHelper.h"
#include "SlAiStyle.h"
#include "SlAiMenuWidgetStyle.h"
#include "SlAiGameWidgetStyle.h"
#include "Sound/SoundCue.h"

TSharedPtr<SlAiDataHandle> SlAiDataHandle::DataInstance = nullptr;

void SlAiDataHandle::Initialize()
{
	if (!DataInstance.IsValid())
		DataInstance = Create();
}

TSharedPtr<SlAiDataHandle> SlAiDataHandle::Get()
{
	Initialize();
	return DataInstance;
}

void SlAiDataHandle::ChangeLocalizationCulture(ECultureTeam Culture)
{
	switch (Culture)
	{
	case ECultureTeam::EN:
		FInternationalization::Get().SetCurrentCulture(TEXT("en"));
		break;
	case ECultureTeam::ZH:
		FInternationalization::Get().SetCurrentCulture(TEXT("zh"));
		break;
	}
	CurrentCulture = Culture;
	SlAiSingleton<SlAiJsonHandle>::Get()->UpdateRecodeData(GetEnumValueAsString<ECultureTeam>(FString("ECultureTeam"), CurrentCulture), MusicVolume, SoundVolume, &RecordDataList);
}

void SlAiDataHandle::ResetMenuVolume(float MusicVol, float SoundVol)
{
	if (MusicVol > 0)
	{
		MusicVolume = MusicVol;
		//ѭ�����ñ�������
		for (TArray<USoundCue*>::TIterator It(MenuAudioResource.Find("Music")->CreateIterator()); It; ++It)
		{
			//��������
			(*It)->VolumeMultiplier = MusicVolume;
		}
	}

	if (SoundVol > 0)
	{
		SoundVolume = SoundVol;
		for (TArray<USoundCue*>::TIterator It(MenuAudioResource.Find("Sound")->CreateIterator()); It; ++It)
		{
			//��������
			(*It)->VolumeMultiplier = SoundVolume;
		}
	}

	SlAiSingleton<SlAiJsonHandle>::Get()->UpdateRecodeData(GetEnumValueAsString<ECultureTeam>(FString("ECultureTeam"), CurrentCulture), MusicVolume, SoundVolume, &RecordDataList);
}

TSharedRef<SlAiDataHandle> SlAiDataHandle::Create()
{
	TSharedRef<SlAiDataHandle> DataRef = MakeShareable(new SlAiDataHandle());
	return DataRef;
}

void SlAiDataHandle::InitRecordData()
{
	RecordName = FString("");

	//��ȡ����
	FString Culture;
	//��ȡ�浵����
	SlAiSingleton<SlAiJsonHandle>::Get()->RecordDataJsonRead(Culture, MusicVolume, SoundVolume, RecordDataList);
	
	//��ʼ������
	ChangeLocalizationCulture(GetEnumValueFromString<ECultureTeam>(FString("ECultureTeam"), Culture));
	
	//���һ��
	SlAiHelper::Debug(Culture + FString("--") + FString::SanitizeFloat(MusicVolume) + FString("--") + FString::SanitizeFloat(SoundVolume), 20.f);
	//ѭ����ȡRecordDataList
	for (TArray<FString>::TIterator It(RecordDataList); It; ++It)
		SlAiHelper::Debug(*It, 20.f);
}

void SlAiDataHandle::InitializedMenuAudio()
{
	//��ȡMenuStyle
	MenuStyle = &SlAiStyle::Get().GetWidgetStyle<FSlAiMenuStyle>("BPSlAiMenuStyle");

	//��ȡ��Դ�ļ�����Դ�б�
	TArray<USoundCue*> MusicList;
	MusicList.Add(Cast<USoundCue>(MenuStyle->MenuBackgroundMusic.GetResourceObject()));

	TArray<USoundCue*> SoundList;
	SoundList.Add(Cast<USoundCue>(MenuStyle->MenuItemChangeSound.GetResourceObject()));
	SoundList.Add(Cast<USoundCue>(MenuStyle->ExitGameSound.GetResourceObject()));
	SoundList.Add(Cast<USoundCue>(MenuStyle->StartGameSound.GetResourceObject()));

	//�����Դ��Map
	MenuAudioResource.Add(FString("Music"), MusicList);
	MenuAudioResource.Add(FString("Sound"), SoundList);

	//����һ������
	ResetMenuVolume(MusicVolume, SoundVolume);
}

void SlAiDataHandle::InitializeGameData()
{
	//��ʼ����Ʒ����
	InitializeObjectAttr();
	//��ʼ����Դ����
	InitializeResourceAttr();
}


void SlAiDataHandle::InitializeObjectAttr()
{
	SlAiSingleton<SlAiJsonHandle>::Get()->ObjectAttrJsonRead(ObjectAttrMap);
	GameStyle = &SlAiStyle::Get().GetWidgetStyle<FSlAiGameStyle>("BPSlAiGameStyle");

	//����ˢ����
	ObjectBrushList.Add(&GameStyle->EmptyBrush);
	ObjectBrushList.Add(&GameStyle->ObjectBrush_1);
	ObjectBrushList.Add(&GameStyle->ObjectBrush_2);
	ObjectBrushList.Add(&GameStyle->ObjectBrush_3);
	ObjectBrushList.Add(&GameStyle->ObjectBrush_4);
	ObjectBrushList.Add(&GameStyle->ObjectBrush_5);
	ObjectBrushList.Add(&GameStyle->ObjectBrush_6);
	ObjectBrushList.Add(&GameStyle->ObjectBrush_7);
}

void SlAiDataHandle::InitializeResourceAttr()
{
	SlAiSingleton<SlAiJsonHandle>::Get()->ResourceAttrJsonRead(ResourceAttrMap);
}

SlAiDataHandle::SlAiDataHandle()
{
	//��ʼ��Ϊ�浵����
	InitRecordData();
	//��ʼ����������
	InitializedMenuAudio();
}

template<typename TEnum>
FString SlAiDataHandle::GetEnumValueAsString(const FString& Name, TEnum Value)
{
	const UEnum* EnumPtr = FindObject<UEnum>(ANY_PACKAGE, *Name, true);
	if (!EnumPtr)
		return FString("Invalid");
	
	return EnumPtr->GetNameByIndex((int32)Value).ToString();
}

template<typename TEnum>
TEnum SlAiDataHandle::GetEnumValueFromString(const FString& Name, FString Value)
{
	const UEnum* EnumPtr = FindObject<UEnum>(ANY_PACKAGE, *Name, true);
	if (!EnumPtr)
		return TEnum(0);
	return (TEnum)EnumPtr->GetIndexByName(FName(*FString(Value)));
}