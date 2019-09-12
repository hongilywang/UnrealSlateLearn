// Fill out your copyright notice in the Description page of Project Settings.


#include "SlAiPackageManager.h"

//��ʼ�Ե���ָ�븳ֵ
TSharedPtr<SlAiPackageManager> SlAiPackageManager::PackageInstance = nullptr;

SlAiPackageManager::SlAiPackageManager()
{
	//��ʼ����Ʒ������
	ObjectIndex = 3;
	ObjectNum = 35;
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
		OutputContainer->CompoundInput.BindRaw(this, &SlAiPackageManager::CompoundInput);
		OutputContainer->ThrowObject.BindRaw(this, &SlAiPackageManager::ThrowObject);
		break;
	case EContainerType::Input:
		Container->CompoundInput.BindRaw(this, &SlAiPackageManager::CompoundInput);
		InputContainerList.Add(Container);
		break;
	case EContainerType::Normal:
		NormalContainerList.Add(Container);
		break;
	case EContainerType::Shortcut:
		Container->PackShortChange.BindRaw(this, &SlAiPackageManager::PackShortChange);
		ShortcutContainerList.Add(Container);
		break;
	}
}

void SlAiPackageManager::UpdateHovered(FVector2D MousePos, FGeometry PackGeo)
{
	//�Ȼ�ȡ��ͣ������
	TSharedPtr<SSlAiContainerBaseWidget> CurrHoveredCon = LocateContainer(MousePos, PackGeo);
	
	if (CurrHoveredCon.IsValid())
	{
		if (LastHoveredCon.IsValid() && LastHoveredCon.Get() == CurrHoveredCon.Get())
			return;

		CurrHoveredCon->UpdateHovered(true);
		if (LastHoveredCon.IsValid())
			LastHoveredCon->UpdateHovered(false);
	}
	else if (LastHoveredCon.IsValid())
	{
		LastHoveredCon->UpdateHovered(false);
	}
	LastHoveredCon = CurrHoveredCon;
}

void SlAiPackageManager::LeftOption(FVector2D MousePos, FGeometry PackGeo)
{
	//�Ȼ�ȡ���������
	TSharedPtr<SSlAiContainerBaseWidget> ClickedContainer = LocateContainer(MousePos, PackGeo);
	if (ClickedContainer.IsValid())
	{
		ClickedContainer->LeftOperate(ObjectIndex, ObjectNum, ObjectIndex, ObjectNum);
	}
	//������������ڲ���������Ʒ��Ϊ��
	if (!ClickedContainer.IsValid() && ObjectIndex != 0)
	{
		//������Ʒ
		ThrowObject(ObjectIndex, ObjectNum);

		//������Ʒ
		ObjectIndex = ObjectNum = 0;
	}
}

void SlAiPackageManager::RightOption(FVector2D MousePos, FGeometry PackGeo)
{
	//�Ȼ�ȡ���������
	TSharedPtr<SSlAiContainerBaseWidget> ClickedContainer = LocateContainer(MousePos, PackGeo);
	if (ClickedContainer.IsValid())
	{
		ClickedContainer->RightOperate(ObjectIndex, ObjectNum, ObjectIndex, ObjectNum);
	}
}

TSharedRef<SlAiPackageManager> SlAiPackageManager::Create()
{
	TSharedRef<SlAiPackageManager> PackageRef = MakeShareable(new SlAiPackageManager());
	return PackageRef;
}

TSharedPtr<SSlAiContainerBaseWidget> SlAiPackageManager::LocateContainer(FVector2D MousePos, FGeometry PackGeo)
{
	//�����ҵ�ָ��������
	for (TArray<TSharedPtr<SSlAiContainerBaseWidget>>::TIterator It(ShortcutContainerList); It; ++It)
	{
		//��ȡ����
		FVector2D StartPos = PackGeo.AbsoluteToLocal((*It)->GetCachedGeometry().AbsolutePosition);
		FVector2D EndPos = StartPos + FVector2D(80.f, 80.f);
		if (MousePos.X >= StartPos.X && MousePos.X <= EndPos.X && MousePos.Y >= StartPos.Y && MousePos.Y <= EndPos.Y)
			return *It;
	}

	for (TArray<TSharedPtr<SSlAiContainerBaseWidget>>::TIterator It(NormalContainerList); It; ++It)
	{
		//��ȡ����
		FVector2D StartPos = PackGeo.AbsoluteToLocal((*It)->GetCachedGeometry().AbsolutePosition);
		FVector2D EndPos = StartPos + FVector2D(80.f, 80.f);
		if (MousePos.X >= StartPos.X && MousePos.X <= EndPos.X && MousePos.Y >= StartPos.Y && MousePos.Y <= EndPos.Y)
			return *It;
	}

	for (TArray<TSharedPtr<SSlAiContainerBaseWidget>>::TIterator It(InputContainerList); It; ++It)
	{
		//��ȡ����
		FVector2D StartPos = PackGeo.AbsoluteToLocal((*It)->GetCachedGeometry().AbsolutePosition);
		FVector2D EndPos = StartPos + FVector2D(80.f, 80.f);
		if (MousePos.X >= StartPos.X && MousePos.X <= EndPos.X && MousePos.Y >= StartPos.Y && MousePos.Y <= EndPos.Y)
			return *It;
	}

	//�������
	FVector2D StartPos = PackGeo.AbsoluteToLocal(OutputContainer->GetCachedGeometry().AbsolutePosition);
	FVector2D EndPos = StartPos + FVector2D(80.f, 80.f);
	if (MousePos.X >= StartPos.X && MousePos.X <= EndPos.X && MousePos.Y >= StartPos.Y && MousePos.Y <= EndPos.Y)
		return OutputContainer;

	//���ؿ�ָ��
	return nullptr;
}

void SlAiPackageManager::ThrowObject(int ObjectID, int Num)
{
	PlayerThrowObject.ExecuteIfBound(ObjectID, Num);
}

void SlAiPackageManager::PackShortChange(int ShortcutID, int ObjectID, int ObjectNum)
{
	ChangeHandObject.ExecuteIfBound(ShortcutID, ObjectID, ObjectNum);
}


void SlAiPackageManager::CompoundOutput(int ObjectID, int Num)
{
}

void SlAiPackageManager::CompoundInput()
{
}

