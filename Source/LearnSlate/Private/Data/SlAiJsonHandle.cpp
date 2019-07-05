// Fill out your copyright notice in the Description page of Project Settings.


#include "SlAiJsonHandle.h"
#include "SlAiHelper.h"

SlAiJsonHandle::SlAiJsonHandle()
{
	RecordDataFileName = FString("RecordData.json");
	RelativePath = FString("Res/ConfigData/");
}

void SlAiJsonHandle::RecordDataJsonRead(FString& Culture, float& MusicVolume, float& SoundVolume, TArray<FString>& RecordDataList)
{
	FString JsonValue;
	LoadStringFromFile(RecordDataFileName, RelativePath, JsonValue);
	
	TArray<TSharedPtr<FJsonValue>> JsonParsed;
	TSharedRef<TJsonReader<TCHAR>> JsonReader = TJsonReaderFactory<TCHAR>::Create(JsonValue);
	if (FJsonSerializer::Deserialize(JsonReader, JsonParsed))
	{
		//获取数据
		Culture = JsonParsed[0]->AsObject()->GetStringField(FString("Culture"));
		MusicVolume = JsonParsed[1]->AsObject()->GetNumberField(FString("MusicVolume"));
		SoundVolume = JsonParsed[2]->AsObject()->GetNumberField(FString("SoundVolume"));
		//获取存档数据
		TArray<TSharedPtr<FJsonValue>> RecordDataArray = JsonParsed[3]->AsObject()->GetArrayField(FString("RecordData"));
		for (int i = 0; i < RecordDataArray.Num(); ++i)
		{
			FString RecordDataName = RecordDataArray[i]->AsObject()->GetStringField(FString::FromInt(i));
			RecordDataList.Add(RecordDataName);
		}
	}
	else
	{
		SlAiHelper::Debug(FString("Deserialize Failed"));
	}
}

bool SlAiJsonHandle::LoadStringFromFile(const FString& FileName, const FString& RelaPath, FString& ResultString)
{
	if (FileName.IsEmpty())
		return false;

	//获取绝对路径
	FString AbsoPath = FPaths::GameContentDir() + RelaPath + FileName;

	if (!FPaths::FileExists(AbsoPath))
	{
		SlAiHelper::Debug(FString("File Not Exist") + AbsoPath);
		return false;
	}

	if (FFileHelper::LoadFileToString(ResultString, *AbsoPath))
		return true;
	else
	{
		SlAiHelper::Debug(FString("Load Error") + AbsoPath);
		return false;
	}
}

bool SlAiJsonHandle::GetFStringInJsonData(const TSharedPtr<FJsonObject>& JsonObj, FString& JsonStr)
{
	if (JsonObj.IsValid() && JsonObj->Values.Num() > 0)
	{
		TSharedRef<TJsonWriter<TCHAR>> JsonWrite = TJsonWriterFactory<TCHAR>::Create(&JsonStr);
		FJsonSerializer::Serialize(JsonObj.ToSharedRef(), JsonWrite);
		return true;
	}

	return false;
}

bool SlAiJsonHandle::WriteFileWithJsonData(const FString& JsonStr, const FString& RelaPath, const FString& FileName)
{
	if (JsonStr.IsEmpty())
		return false;

	if (FileName.IsEmpty())
		return false;

	FString AbsoPath = FPaths::GameConfigDir() + RelaPath + FileName;
	if (FFileHelper::SaveStringToFile(JsonStr, *AbsoPath))
		return true;
	else
	{
		SlAiHelper::Debug(FString("Save") + AbsoPath + FString("-->Failed"), 10.f);
		return false;
	}

}
