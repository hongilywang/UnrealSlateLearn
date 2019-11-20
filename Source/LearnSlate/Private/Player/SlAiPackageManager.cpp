// Fill out your copyright notice in the Description page of Project Settings.


#include "SlAiPackageManager.h"
#include "SlAiDataHandle.h"

//��ʼ�Ե���ָ�븳ֵ
TSharedPtr<SlAiPackageManager> SlAiPackageManager::PackageInstance = nullptr;

SlAiPackageManager::SlAiPackageManager()
{
	//��ʼ����Ʒ������
	ObjectIndex = 0;
	ObjectNum = 0;
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
		OutputContainer->CompoundOutput.BindRaw(this, &SlAiPackageManager::CompoundOutput);
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

void SlAiPackageManager::PackShortChange(int ShortcutID, int ObjectID, int Num)
{
	ChangeHandObject.ExecuteIfBound(ShortcutID, ObjectID, Num);
}


bool SlAiPackageManager::MultiplyAble(int ObjectID)
{
	//��ȡ��Ʒ����
	TSharedPtr<ObjectAttribute> ObjectAttr = *SlAiDataHandle::Get()->ObjectAttrMap.Find(ObjectID);
	//�����Ƿ����������߹���
	return (ObjectAttr->ObjectType != EObjectType::Tool && ObjectAttr->ObjectType != EObjectType::Weapon);
}

bool SlAiPackageManager::SearchFreeSpace(int ObjectID, TSharedPtr<SSlAiContainerBaseWidget>& FreeContainer)
{
	//����������
	TSharedPtr<SSlAiContainerBaseWidget> EmptyContainer;
	//���ҿ����
	for (TArray<TSharedPtr<SSlAiContainerBaseWidget>>::TIterator It(ShortcutContainerList); It; ++It)
	{
		if (!EmptyContainer.IsValid())
		{
			if ((*It)->IsEmpty())
				EmptyContainer = *It;
		}
		
		if (!FreeContainer.IsValid())
		{
			if ((*It)->IsRemainSpace(ObjectID))
			{
				FreeContainer = *It;
				return true;
			}
		}
	}

	//��������
	for (TArray<TSharedPtr<SSlAiContainerBaseWidget>>::TIterator It(NormalContainerList); It; ++It)
	{
		if (!EmptyContainer.IsValid())
		{
			if ((*It)->IsEmpty())
				EmptyContainer = *It;
		}

		if (!FreeContainer.IsValid())
		{
			if ((*It)->IsRemainSpace(ObjectID))
			{
				FreeContainer = *It;
				return true;
			}
		}
	}

	//������е�����˵����Ҫָ��������
	if (EmptyContainer.IsValid())
	{
		FreeContainer = EmptyContainer;
		return true;
	}

	return false;
}

bool SlAiPackageManager::SearchFreeSpace(int ObjectID)
{
	TSharedPtr<SSlAiContainerBaseWidget> FreeContainer;
	return SearchFreeSpace(ObjectID, FreeContainer);
}

void SlAiPackageManager::AddObject(int ObjectID)
{
	TSharedPtr<SSlAiContainerBaseWidget> FreeContainer;
	if (SearchFreeSpace(ObjectID, FreeContainer))
	{
		//�����Ʒ������
		FreeContainer->AddObject(ObjectID);
	}
}

bool SlAiPackageManager::EatUpEvent(int ShortcutID)
{
	//��ȡ��Ʒ����
	TSharedPtr<ObjectAttribute> ObjectAttr = *SlAiDataHandle::Get()->ObjectAttrMap.Find(ShortcutContainerList[ShortcutID]->GetIndex());
	//�Ƿ���ʳ��
	if (ObjectAttr->ObjectType == EObjectType::Food)
	{
		//������Ʒ����
		int NewNum = (ShortcutContainerList[ShortcutID]->GetNum() - 1) < 0 ? 0 : (ShortcutContainerList[ShortcutID]->GetNum() - 1);
		//��ƷID
		int NewIndex = NewNum == 0 ? 0 : ShortcutContainerList[ShortcutID]->GetIndex();
		//������������
		ShortcutContainerList[ShortcutID]->ResetContainerPara(NewIndex, NewNum);
		return true;
	}

	return false;
}

void SlAiPackageManager::LoadRecord(TArray<int32>* InputIndex, TArray<int32>* InputNum, TArray<int32>* NormalIndex, TArray<int32>* NormalNum, TArray<int32>* ShortcutIndex, TArray<int32>* ShortcutNum)
{
	for (int i = 0; i < InputContainerList.Num(); ++i)
	{
		if ((*InputIndex)[i] != 0)
			InputContainerList[i]->ResetContainerPara((*InputIndex)[i], (*InputNum)[i]);
	}
	for (int i = 0; i < NormalContainerList.Num(); ++i)
	{
		if ((*NormalIndex)[i] != 0)
			NormalContainerList[i]->ResetContainerPara((*NormalIndex)[i], (*NormalNum)[i]);
	}
	for (int i = 0; i < ShortcutContainerList.Num(); ++i)
	{
		if ((*ShortcutIndex)[i] != 0)
			ShortcutContainerList[i]->ResetContainerPara((*ShortcutIndex)[i], (*ShortcutNum)[i]);
	}
}

void SlAiPackageManager::SaveData(TArray<int32>& InputIndex, TArray<int32>& InputNum, TArray<int32>& NormalIndex, TArray<int32>& NormalNum, TArray<int32>& ShortcutIndex, TArray<int32>& ShortcutNum)
{
	for (int i = 0; i < InputContainerList.Num(); ++i)
	{
		InputIndex.Add(InputContainerList[i]->GetIndex());
		InputNum.Add(InputContainerList[i]->GetNum());
	}
	for (int i = 0; i < NormalContainerList.Num(); ++i)
	{
		NormalIndex.Add(NormalContainerList[i]->GetIndex());
		NormalNum.Add(NormalContainerList[i]->GetNum());
	}
	for (int i = 0; i < ShortcutContainerList.Num(); ++i)
	{
		ShortcutIndex.Add(ShortcutContainerList[i]->GetIndex());
		ShortcutNum.Add(ShortcutContainerList[i]->GetNum());
	}
}

void SlAiPackageManager::CompoundOutput(int ObjectID, int Num)
{
	//�������Ϊ0
	if (ObjectID == 0)
		return;

	//�ϳɱ�ṹ����
	TArray<int> TableMap;
	for (TArray<TSharedPtr<SSlAiContainerBaseWidget>>::TIterator It(InputContainerList); It; ++It)
	{
		TableMap.Add((*It)->GetIndex());
	}
	TableMap.Add(ObjectID);
	//������������
	TArray<int> ExpendMap;
	for (TArray<TSharedPtr<CompoundTable>>::TIterator It(SlAiDataHandle::Get()->CompoundTableMap); It; ++It)
	{
		if ((*It)->DetectExpend(&TableMap, Num, ExpendMap))
			break;
	}

	//�����������Ԫ���Ƿ���9
	if (ExpendMap.Num() != 9)
		return;

	for (int i = 0; i < 9; ++i)
	{
		int InputID = (InputContainerList[i]->GetNum() - ExpendMap[i] <= 0) ? 0 : InputContainerList[i]->GetIndex();
		int InputNum = (InputID == 0) ? 0 : (InputContainerList[i]->GetNum() - ExpendMap[i]);
		InputContainerList[i]->ResetContainerPara(InputID, InputNum);
	}
}

void SlAiPackageManager::CompoundInput()
{
	//��ȡ�ϳ�̨9����������Ʒid������д����������
	TArray<int> IDMap;
	TArray<int> NumMap;
	for (TArray<TSharedPtr<SSlAiContainerBaseWidget>>::TIterator It(InputContainerList); It; ++It)
	{
		IDMap.Add((*It)->GetIndex());
		NumMap.Add((*It)->GetNum());
	}

	int OutputIndex = 0;
	int OutputNum = 0;
	for (TArray<TSharedPtr<CompoundTable>>::TIterator It(SlAiDataHandle::Get()->CompoundTableMap); It; ++It)
	{
		(*It)->DetectTable(&IDMap, &NumMap, OutputIndex, OutputNum);
		//����������ˣ�ֱ������ѭ��
		if (OutputIndex != 0 && OutputNum != 0)
			break;
	}

	//��OutputContainer���и�ֵ
	if (MultiplyAble(OutputIndex))
		OutputContainer->ResetContainerPara(OutputIndex, OutputNum);
	else
		OutputContainer->ResetContainerPara(OutputIndex, 1);
}

