// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SSlAiContainerBaseWidget.h"
#include "SlAiTypes.h"

/**
 * 
 */
class LEARNSLATE_API SlAiPackageManager
{
public:
	SlAiPackageManager();

	static void Initialize();

	static TSharedPtr<SlAiPackageManager> Get();

	//�������
	void InsertContainer(TSharedPtr<SSlAiContainerBaseWidget> Container, EContainerType::Type InsertType);

private:
	//����ʵ������
	static TSharedRef<SlAiPackageManager> Create();

private:
	//����ָ��
	static TSharedPtr<SlAiPackageManager> PackageInstance;

	//�����б�
	TArray<TSharedPtr<SSlAiContainerBaseWidget>> InputContainerList;
	TArray<TSharedPtr<SSlAiContainerBaseWidget>> NormalContainerList;
	TArray<TSharedPtr<SSlAiContainerBaseWidget>> ShortcutContainerList;
	//��������ֻ��һ��
	TSharedPtr<SSlAiContainerBaseWidget> OutputContainer;
};
