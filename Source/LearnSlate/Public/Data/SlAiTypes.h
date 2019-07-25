// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

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