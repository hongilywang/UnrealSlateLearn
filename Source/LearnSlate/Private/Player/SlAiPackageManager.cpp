// Fill out your copyright notice in the Description page of Project Settings.


#include "SlAiPackageManager.h"

//��ʼ�Ե���ָ�븳ֵ
TSharedPtr<SlAiPackageManager> SlAiPackageManager::PackageInstance = nullptr;

SlAiPackageManager::SlAiPackageManager()
{
}

void SlAiPackageManager::Initialize()
{
	if (!PackageInstance.IsValid())
		PackageInstance = Create();
}

TSharedPtr<SlAiPackageManager> SlAiPackageManager::Get()
{
	Initialize();
	return PackageInstance;
}

void SlAiPackageManager::InsertContainer(TSharedPtr<SSlAiContainerBaseWidget> Container, EContainerType::Type InsertType)
{
	switch (InsertType)
	{
	case EContainerType::Output:
		OutputContainer = Container;
		break;
	case EContainerType::Input:
		InputContainerList.Add(Container);
		break;
	case EContainerType::Normal:
		NormalContainerList.Add(Container);
		break;
	case EContainerType::Shortcut:
		ShortcutContainerList.Add(Container);
		break;
	}
}

TSharedRef<SlAiPackageManager> SlAiPackageManager::Create()
{
	TSharedRef<SlAiPackageManager> PackageRef = MakeShareable(new SlAiPackageManager());
	return PackageRef;
}
