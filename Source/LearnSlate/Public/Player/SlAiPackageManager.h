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

	//������ͣ��������ɫ
	void UpdateHovered(FVector2D MousePos, FGeometry PackGeo);

	//����¼������������λ�ú�PackageWidget��Geometry
	void LeftOption(FVector2D MousePos, FGeometry PackGeo);

	//�Ҽ��¼������������λ�ú�PackageWidget��Geometry
	void RightOption(FVector2D MousePos, FGeometry PackGeo);

public:
	//�����ƷID
	int ObjectIndex;
	//�����Ʒ����
	int ObjectNum;

	//������Ʒί�У��󶨵ķ�����PlayerState��PlayerThrowObject
	FThrowObject PlayerThrowObject;
	//�޸Ŀ������Ϣί��
	FPackShortChange ChangeHandObject;

private:
	//����ʵ������
	static TSharedRef<SlAiPackageManager> Create();

	//��ȡ���ָ�������
	TSharedPtr<SSlAiContainerBaseWidget> LocateContainer(FVector2D MousePos, FGeometry PackGeo);

	//������Ʒ�¼�
	void ThrowObject(int ObjectID, int Num);
	//�ϳ���ǰ�¼�
	void CompoundOutput(int ObjectID, int Num);
	//�ϳ������¼�
	void CompoundInput();
	//�����ί���¼�
	void PackShortChange(int ShortcutID, int ObjectID, int ObjectNum);

private:
	//����ָ��
	static TSharedPtr<SlAiPackageManager> PackageInstance;

	//�����б�
	TArray<TSharedPtr<SSlAiContainerBaseWidget>> InputContainerList;
	TArray<TSharedPtr<SSlAiContainerBaseWidget>> NormalContainerList;
	TArray<TSharedPtr<SSlAiContainerBaseWidget>> ShortcutContainerList;
	//��������ֻ��һ��
	TSharedPtr<SSlAiContainerBaseWidget> OutputContainer;

	//��һ����ͣ������
	TSharedPtr<SSlAiContainerBaseWidget> LastHoveredCon;
};
