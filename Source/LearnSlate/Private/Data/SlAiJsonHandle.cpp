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

void SlAiJsonHandle::UpdateRecodeData(FString Culture, float MusicVolume, float SoundVolume, TArray<FString>* RecordDataList)
{
	TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject);
	TArray<TSharedPtr<FJsonValue>> BaseDataArray;

	TSharedPtr<FJsonObject> CultureObject = MakeShareable(new FJsonObject);
	CultureObject->SetStringField("Culture", Culture);
	TSharedPtr<FJsonValueObject> CultureValue = MakeShareable(new FJsonValueObject(CultureObject));

	TSharedPtr<FJsonObject> MusicVolumeObject = MakeShareable(new FJsonObject);
	MusicVolumeObject->SetNumberField("MusicVolume", MusicVolume);
	TSharedPtr<FJsonValueObject> MusicVolumeValue = MakeShareable(new FJsonValueObject(MusicVolumeObject));

	TSharedPtr<FJsonObject> SoundVolumeObject = MakeShareable(new FJsonObject);
	SoundVolumeObject->SetNumberField("SoundVolume", SoundVolume);
	TSharedPtr<FJsonValueObject> SoundVolumeValue = MakeShareable(new FJsonValueObject(SoundVolumeObject));

	TArray<TSharedPtr<FJsonValue>> RecordDataArray;
	for (int i = 0; i < RecordDataList->Num(); ++i)
	{
		TSharedPtr<FJsonObject> RecordItem = MakeShareable(new FJsonObject);
		RecordItem->SetStringField(FString::FromInt(i), (*RecordDataList)[i]);
		TSharedPtr<FJsonValueObject> RecordDataValue = MakeShareable(new FJsonValueObject(RecordItem));
		RecordDataArray.Add(RecordDataValue);
	}

	TSharedPtr<FJsonObject> RecordDataObject = MakeShareable(new FJsonObject);
	RecordDataObject->SetArrayField("RecordData", RecordDataArray);
	TSharedPtr<FJsonValueObject> RecordDataValue = MakeShareable(new FJsonValueObject(RecordDataObject));

	BaseDataArray.Add(CultureValue);
	BaseDataArray.Add(MusicVolumeValue);
	BaseDataArray.Add(SoundVolumeValue);
	BaseDataArray.Add(RecordDataValue);

	JsonObject->SetArrayField("T", BaseDataArray);

	FString JsonStr;
	GetFStringInJsonData(JsonObject, JsonStr);

	//去掉多余字符
	JsonStr.RemoveAt(0, 8);
	JsonStr.RemoveFromEnd(FString("}"));

	//写入文件
	WriteFileWithJsonData(JsonStr, RelativePath, RecordDataFileName);
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

	FString AbsoPath = FPaths::GameContentDir() + RelaPath + FileName;
	if (FFileHelper::SaveStringToFile(JsonStr, *AbsoPath))
	{
		return true;
	}
	else
	{
		SlAiHelper::Debug(FString("Save") + AbsoPath + FString("-->Failed"), 10.f);
		return false;
	}

}
