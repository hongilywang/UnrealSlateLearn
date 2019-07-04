// Fill out your copyright notice in the Description page of Project Settings.


#include "SlAiDataHandle.h"
#include "Internationalization.h"

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
}

void SlAiDataHandle::ResetMenuVolume(float MusicVol, float SoundVol)
{
	if (MusicVol > 0)
		MusicVolume = MusicVol;

	if (SoundVol > 0)
		SoundVol = SoundVol;
}

TSharedRef<SlAiDataHandle> SlAiDataHandle::Create()
{
	TSharedRef<SlAiDataHandle> DataRef = MakeShareable(new SlAiDataHandle());
	return DataRef;
}

SlAiDataHandle::SlAiDataHandle()
{
	//��ʼ��Ϊ����
	CurrentCulture = ECultureTeam::ZH;
	MusicVolume = 0.5f;
	SoundVolume = 0.5f;
}

template<typename TEnum>
FString SlAiDataHandle::GetEnumValueAsString(const FString& Name, TEnum Value)
{
	const UEnum* EnumPtr = FindObject<UEnum>(ANY_PACKAGE, *Name, true);
	if (!EnumPtr)
		return FString("Invalid");
	
	return EnumPtr->GetEnumName((int32)Value);
}

template<typename TEnum>
TEnum SlAiDataHandle::GetEnumValueFromString(const FString& Name, FString Value)
{
	const UEnum* EnumPtr = FindObject<UEnum>(ANY_PACKAGE, *Name, true);
	if (!EnumPtr)
		return TEnum(0);
	return (TEnum)EnumPtr->GetIndexByName(FName(*FString(Value)));
}