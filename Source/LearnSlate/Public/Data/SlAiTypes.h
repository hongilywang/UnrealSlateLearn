// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */

/**
 * UENUM��UE4���涨���ö�٣����Խ������ö�ټ��뷴���У�����ͼ�п���ʹ��
 */
UENUM()
enum class ECultureTeam : uint8
{
	EN = 0,
	ZH
};

//Menu��ť������
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

//menu��������
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

//Menu����״̬ö��
namespace EMenuAnim
{
	enum Type
	{
		Stop,	//ֹͣ����
		Close,	//�ر�Menu
		Open,	//��Menu
	};
}

//�ӽ�ģʽ
namespace EGameViewMode
{
	enum Type
	{
		First,
		Third,
	};
}

//�ϰ�����״̬
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

//��Ʒ����
namespace EObjectType
{
	enum Type
	{
		Normal = 0, //��ͨ��Ʒ
		Food,	//ʳ��
		Tool,	//����
		Weapon	//����
	};
}

//��Ʒ���Խṹ��
struct ObjectAttribute 
{
	FText EN; //Ӣ����
	FText ZH; //������
	EObjectType::Type ObjectType; //��Ʒ����
	int PlantAttack;	//��ֲ��Ĺ�����
	int MetalAttack;	//�Խ����Ĺ�����
	int AnimalAttack;	//�Զ���Ĺ�����
	int AffectRange;	//��������
	FString TexPath;	//ͼƬ·����Դ

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