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
#include "AudioDevice.h"
#include "Sound/SoundMix.h"
#include "Sound/SoundClass.h"

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
		//循环设置背景音量
		for (TArray<USoundCue*>::TIterator It(MenuAudioResource.Find("Music")->CreateIterator()); It; ++It)
		{
			//设置音量
			(*It)->VolumeMultiplier = MusicVolume;
		}
	}

	if (SoundVol > 0)
	{
		SoundVolume = SoundVol;
		for (TArray<USoundCue*>::TIterator It(MenuAudioResource.Find("Sound")->CreateIterator()); It; ++It)
		{
			//设置音量
			(*It)->VolumeMultiplier = SoundVolume;
		}
	}

	SlAiSingleton<SlAiJsonHandle>::Get()->UpdateRecodeData(GetEnumValueAsString<ECultureTeam>(FString("ECultureTeam"), CurrentCulture), MusicVolume, SoundVolume, &RecordDataList);
}

void SlAiDataHandle::ResetGameVolume(float MusicVol, float SoundVol)
{
	if (MusicVol > 0)
	{
		MusicVolume = MusicVol;
		//使用混音器设置
		AudioDevice->SetSoundMixClassOverride(SlAiSoundMix, SlAiMusicClass, MusicVolume, 1.f, 0.2f, false);
	}

	if (SoundVol > 0)
	{
		SoundVolume = SoundVol;
		//使用混音器设置
		AudioDevice->SetSoundMixClassOverride(SlAiSoundMix, SlAiSoundClass, SoundVolume, 1.f, 0.2f, false);
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

	//获取语言
	FString Culture;
	//读取存档数据
	SlAiSingleton<SlAiJsonHandle>::Get()->RecordDataJsonRead(Culture, MusicVolume, SoundVolume, RecordDataList);
	
	//初始化语言
	ChangeLocalizationCulture(GetEnumValueFromString<ECultureTeam>(FString("ECultureTeam"), Culture));
	
	//输出一下
	SlAiHelper::Debug(Culture + FString("--") + FString::SanitizeFloat(MusicVolume) + FString("--") + FString::SanitizeFloat(SoundVolume), 20.f);
	//循环读取RecordDataList
	for (TArray<FString>::TIterator It(RecordDataList); It; ++It)
		SlAiHelper::Debug(*It, 20.f);
}

void SlAiDataHandle::InitializedMenuAudio()
{
	//获取MenuStyle
	MenuStyle = &SlAiStyle::Get().GetWidgetStyle<FSlAiMenuStyle>("BPSlAiMenuStyle");

	//获取资源文件到资源列表
	TArray<USoundCue*> MusicList;
	MusicList.Add(Cast<USoundCue>(MenuStyle->MenuBackgroundMusic.GetResourceObject()));

	TArray<USoundCue*> SoundList;
	SoundList.Add(Cast<USoundCue>(MenuStyle->MenuItemChangeSound.GetResourceObject()));
	SoundList.Add(Cast<USoundCue>(MenuStyle->ExitGameSound.GetResourceObject()));
	SoundList.Add(Cast<USoundCue>(MenuStyle->StartGameSound.GetResourceObject()));

	//添加资源到Map
	MenuAudioResource.Add(FString("Music"), MusicList);
	MenuAudioResource.Add(FString("Sound"), SoundList);

	//重置一下声音
	ResetMenuVolume(MusicVolume, SoundVolume);
}

void SlAiDataHandle::InitializeGameData()
{
	//初始化物品属性
	InitializeObjectAttr();
	//初始化资源属性
	InitializeResourceAttr();
	//初始化合成表
	InitCompoundTableMap();
	//初始化游戏声音数据
	InitializeGameAudio();
}


void SlAiDataHandle::AddNewRecord()
{
	RecordDataList.Add(RecordName);
	//更新Json数据
	SlAiSingleton<SlAiJsonHandle>::Get()->UpdateRecodeData(GetEnumValueAsString<ECultureTeam>(FString("ECultureTeam"), CurrentCulture), MusicVolume, SoundVolume, &RecordDataList);

}

void SlAiDataHandle::InitializeObjectAttr()
{
	SlAiSingleton<SlAiJsonHandle>::Get()->ObjectAttrJsonRead(ObjectAttrMap);
	GameStyle = &SlAiStyle::Get().GetWidgetStyle<FSlAiGameStyle>("BPSlAiGameStyle");

	//填充笔刷数组
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

void SlAiDataHandle::InitCompoundTableMap()
{
	SlAiSingleton<SlAiJsonHandle>::Get()->CompundTableJsonRead(CompoundTableMap);
}

void SlAiDataHandle::InitializeGameAudio()
{
	SlAiSoundClass = LoadObject<USoundClass>(nullptr, TEXT("SoundClass'/Game/Blueprint/Sound/SlAiSoundClass.SlAiSoundClass'"));
	SlAiMusicClass = LoadObject<USoundClass>(nullptr, TEXT("SoundClass'/Game/Blueprint/Sound/SlAiMusicClass.SlAiMusicClass'"));
	SlAiSoundMix = LoadObject<USoundMix>(nullptr, TEXT("SoundMix'/Game/Blueprint/Sound/SlAiSoundMix.SlAiSoundMix'"));

	//获取因为设备
	AudioDevice = GEngine->GetMainAudioDevice();

	//推送混音器到设备
	AudioDevice->PushSoundMixModifier(SlAiSoundMix);

	//根据音量设置一次声音
	ResetGameVolume(MusicVolume, SoundVolume);

}

SlAiDataHandle::SlAiDataHandle()
{
	//初始化为存档数据
	InitRecordData();
	//初始化音乐数据
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