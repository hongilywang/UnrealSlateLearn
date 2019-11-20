// Fill out your copyright notice in the Description page of Project Settings.


#include "SlAiPackageManager.h"
#include "SlAiDataHandle.h"

//开始对单例指针赋值
TSharedPtr<SlAiPackageManager> SlAiPackageManager::PackageInstance = nullptr;

SlAiPackageManager::SlAiPackageManager()
{
	//初始化物品和数量
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
	//先获取悬停的容器
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
	//先获取点击的容器
	TSharedPtr<SSlAiContainerBaseWidget> ClickedContainer = LocateContainer(MousePos, PackGeo);
	if (ClickedContainer.IsValid())
	{
		ClickedContainer->LeftOperate(ObjectIndex, ObjectNum, ObjectIndex, ObjectNum);
	}
	//如果容器不存在并且手上物品不为空
	if (!ClickedContainer.IsValid() && ObjectIndex != 0)
	{
		//丢弃物品
		ThrowObject(ObjectIndex, ObjectNum);

		//重置物品
		ObjectIndex = ObjectNum = 0;
	}
}

void SlAiPackageManager::RightOption(FVector2D MousePos, FGeometry PackGeo)
{
	//先获取点击的容器
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
	//迭代找到指定的容器
	for (TArray<TSharedPtr<SSlAiContainerBaseWidget>>::TIterator It(ShortcutContainerList); It; ++It)
	{
		//获取区域
		FVector2D StartPos = PackGeo.AbsoluteToLocal((*It)->GetCachedGeometry().AbsolutePosition);
		FVector2D EndPos = StartPos + FVector2D(80.f, 80.f);
		if (MousePos.X >= StartPos.X && MousePos.X <= EndPos.X && MousePos.Y >= StartPos.Y && MousePos.Y <= EndPos.Y)
			return *It;
	}

	for (TArray<TSharedPtr<SSlAiContainerBaseWidget>>::TIterator It(NormalContainerList); It; ++It)
	{
		//获取区域
		FVector2D StartPos = PackGeo.AbsoluteToLocal((*It)->GetCachedGeometry().AbsolutePosition);
		FVector2D EndPos = StartPos + FVector2D(80.f, 80.f);
		if (MousePos.X >= StartPos.X && MousePos.X <= EndPos.X && MousePos.Y >= StartPos.Y && MousePos.Y <= EndPos.Y)
			return *It;
	}

	for (TArray<TSharedPtr<SSlAiContainerBaseWidget>>::TIterator It(InputContainerList); It; ++It)
	{
		//获取区域
		FVector2D StartPos = PackGeo.AbsoluteToLocal((*It)->GetCachedGeometry().AbsolutePosition);
		FVector2D EndPos = StartPos + FVector2D(80.f, 80.f);
		if (MousePos.X >= StartPos.X && MousePos.X <= EndPos.X && MousePos.Y >= StartPos.Y && MousePos.Y <= EndPos.Y)
			return *It;
	}

	//输出容器
	FVector2D StartPos = PackGeo.AbsoluteToLocal(OutputContainer->GetCachedGeometry().AbsolutePosition);
	FVector2D EndPos = StartPos + FVector2D(80.f, 80.f);
	if (MousePos.X >= StartPos.X && MousePos.X <= EndPos.X && MousePos.Y >= StartPos.Y && MousePos.Y <= EndPos.Y)
		return OutputContainer;

	//返回空指针
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
	//获取物品属性
	TSharedPtr<ObjectAttribute> ObjectAttr = *SlAiDataHandle::Get()->ObjectAttrMap.Find(ObjectID);
	//返回是否是武器或者工具
	return (ObjectAttr->ObjectType != EObjectType::Tool && ObjectAttr->ObjectType != EObjectType::Weapon);
}

bool SlAiPackageManager::SearchFreeSpace(int ObjectID, TSharedPtr<SSlAiContainerBaseWidget>& FreeContainer)
{
	//空容器引用
	TSharedPtr<SSlAiContainerBaseWidget> EmptyContainer;
	//先找快捷栏
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

	//背包容器
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

	//如果运行到这来说明需要指定空容器
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
		//添加物品到容器
		FreeContainer->AddObject(ObjectID);
	}
}

bool SlAiPackageManager::EatUpEvent(int ShortcutID)
{
	//获取物品属性
	TSharedPtr<ObjectAttribute> ObjectAttr = *SlAiDataHandle::Get()->ObjectAttrMap.Find(ShortcutContainerList[ShortcutID]->GetIndex());
	//是否是食物
	if (ObjectAttr->ObjectType == EObjectType::Food)
	{
		//更新物品数量
		int NewNum = (ShortcutContainerList[ShortcutID]->GetNum() - 1) < 0 ? 0 : (ShortcutContainerList[ShortcutID]->GetNum() - 1);
		//物品ID
		int NewIndex = NewNum == 0 ? 0 : ShortcutContainerList[ShortcutID]->GetIndex();
		//更新容器属性
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
	//如果生成为0
	if (ObjectID == 0)
		return;

	//合成表结构数组
	TArray<int> TableMap;
	for (TArray<TSharedPtr<SSlAiContainerBaseWidget>>::TIterator It(InputContainerList); It; ++It)
	{
		TableMap.Add((*It)->GetIndex());
	}
	TableMap.Add(ObjectID);
	//消耗数量数组
	TArray<int> ExpendMap;
	for (TArray<TSharedPtr<CompoundTable>>::TIterator It(SlAiDataHandle::Get()->CompoundTableMap); It; ++It)
	{
		if ((*It)->DetectExpend(&TableMap, Num, ExpendMap))
			break;
	}

	//如果消耗数组元素是否是9
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
	//获取合成台9个容器的物品id和数量写进两个数组
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
		//如果检测出来了，直接跳出循环
		if (OutputIndex != 0 && OutputNum != 0)
			break;
	}

	//对OutputContainer进行赋值
	if (MultiplyAble(OutputIndex))
		OutputContainer->ResetContainerPara(OutputIndex, OutputNum);
	else
		OutputContainer->ResetContainerPara(OutputIndex, 1);
}

