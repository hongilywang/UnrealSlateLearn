// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SBorder.h"
#include "STextBlock.h"
#include "SlateBrush.h"

/**
 * 
 */

/**
 * UENUM是UE4里面定义的枚举，可以将定义的枚举加入反射中，在蓝图中可以使用
 */
UENUM()
enum class ECultureTeam : uint8
{
	EN = 0,
	ZH
};

//Menu按钮的类型
namespace EMenuItem
{
	enum Type
	{
		None,
		StartGame,
		GameOption,
		QuitGame,
		NewGame,
		LoadRecord,
		StartGameGoBack,
		GameOptionGoBack,
		NewGameGoBack,
		ChooseRecordGoBack,
		EnterGame,
		EnterRecord
	};
}

//menu界面类型
namespace EMenuType
{
	enum Type
	{
		None,
		MainMenu,
		StartGame,
		GameOption,
		NewGame,
		ChooseRecord
	};
}

//Menu动画状态枚举
namespace EMenuAnim
{
	enum Type
	{
		Stop,	//停止动画
		Close,	//关闭Menu
		Open,	//打开Menu
	};
}

//视角模式
namespace EGameViewMode
{
	enum Type
	{
		First,
		Third,
	};
}

//上半身动画状态
namespace EUpperBody
{
	enum Type
	{
		None,
		Punch,
		Hit,
		Fight,
		PickUp,
		Eat
	};
}

//物品类型
namespace EObjectType
{
	enum Type
	{
		Normal = 0, //普通物品
		Food,	//食物
		Tool,	//工具
		Weapon	//武器
	};
}

//物品属性结构体
struct ObjectAttribute 
{
	FText EN; //英文名
	FText ZH; //中文名
	EObjectType::Type ObjectType; //物品类型
	int PlantAttack;	//对植物的攻击力
	int MetalAttack;	//对金属的攻击力
	int AnimalAttack;	//对动物的攻击力
	int AffectRange;	//攻击距离
	FString TexPath;	//图片路径资源

	ObjectAttribute(const FText ENName, const FText ZHName, const EObjectType::Type OT, const int PA, const int MA, const int AA, const int AR, const FString TP)
	{
		EN = ENName;
		ZH = ZHName;
		ObjectType = OT;
		PlantAttack = PA;
		MetalAttack = MA;
		AnimalAttack = AA;
		AffectRange = AR;
		TexPath = TP;
	}
};

//快捷栏容器结构体
struct ShortcutContainer
{
	//物品ID
	int ObjectIndex;
	int ObjectNum;
	TSharedPtr<SBorder> ContainerBorder;
	TSharedPtr<SBorder> ObjectImage;
	TSharedPtr<STextBlock> ObjectNumText;
	const FSlateBrush* NormalContainerBrush;
	const FSlateBrush* ChoosedContainerBrush;
	TArray<const FSlateBrush*>* ObjectBrushList;

	ShortcutContainer(TSharedPtr<SBorder> CB, TSharedPtr<SBorder> OI, TSharedPtr<STextBlock> ONT, const FSlateBrush* NCB, const FSlateBrush* CCB, TArray<const FSlateBrush*>* OBL)
	{
		ContainerBorder = CB;
		ObjectImage = OI;
		ObjectNumText = ONT;
		NormalContainerBrush = NCB;
		ChoosedContainerBrush = CCB;
		ObjectBrushList = OBL;

		//初始化显示设置
		ObjectIndex = 0;
		ObjectNum = 0;
		ContainerBorder->SetBorderImage(NormalContainerBrush);
		ObjectImage->SetBorderImage((*ObjectBrushList)[0]);
	}

	//设置是否选中当前的物品
	int SetChoosed(bool Option)
	{
		if (Option)
			ContainerBorder->SetBorderImage(ChoosedContainerBrush);
		else
			ContainerBorder->SetBorderImage(NormalContainerBrush);

		return ObjectIndex;
	}

	//设置Index
	ShortcutContainer* SetObject(int NewIndex)
	{
		ObjectIndex = NewIndex;
		ObjectImage->SetBorderImage((*ObjectBrushList)[ObjectIndex]);
		return this;
	}

	//设置数量
	ShortcutContainer* SetObjectNum(int Num = 0)
	{
		ObjectNum = Num;
		//如果数量为0或者1，不显示
		if (ObjectNum == 0 || ObjectNum == 1)
			ObjectNumText->SetText(FString(""));
		else
			ObjectNumText->SetText(FString::FromInt(Num));

		return this;
	}
};

//资源类型
namespace EResourceType
{
	enum Type
	{
		Plant = 0,
		Metal,
		Animal
	};
}

//资源属性结构体
struct ResourceAttribute
{
	FText EN;
	FText ZH;
	EResourceType::Type ResourceType;
	int HP;
	TArray<TArray<int>> FlobObjectInfo;

	ResourceAttribute(const FText ENName, const FText ZHName, const EResourceType::Type RT, const int HPValue, TArray<TArray<int>>* FOI)
	{
		EN = ENName;
		ZH = ZHName;
		ResourceType = RT;
		HP = HPValue;

		//将数组原始迭代进本数组
		for (TArray<TArray<int>>::TIterator It(*FOI); It; ++It)
		{
			TArray<int> FlobObjectInfoItem;
			for (TArray<int>::TIterator Ih(*It); Ih; ++Ih)
				FlobObjectInfoItem.Add(*Ih);
			FlobObjectInfo.Add(FlobObjectInfoItem);
		}
	}
};

//Game界面分类
namespace EGameUIType
{
	enum Type
	{
		Game, //游戏模式UI
		Pause, //暂停
		Lose,	//输了，死亡
		Package, //背包
		ChatRoom, //聊天室
	};
}

//背包容器类
namespace EContainerType
{
	enum Type
	{
		Output, //合成表输出
		Input,  //合成表输入
		Normal, //普通容器
		Shortcut, //快捷栏容器
	};
}