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

//����������ṹ��
struct ShortcutContainer
{
	//��ƷID
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

		//��ʼ����ʾ����
		ObjectIndex = 0;
		ObjectNum = 0;
		ContainerBorder->SetBorderImage(NormalContainerBrush);
		ObjectImage->SetBorderImage((*ObjectBrushList)[0]);
	}

	//�����Ƿ�ѡ�е�ǰ����Ʒ
	int SetChoosed(bool Option)
	{
		if (Option)
			ContainerBorder->SetBorderImage(ChoosedContainerBrush);
		else
			ContainerBorder->SetBorderImage(NormalContainerBrush);

		return ObjectIndex;
	}

	//����Index
	ShortcutContainer* SetObject(int NewIndex)
	{
		ObjectIndex = NewIndex;
		ObjectImage->SetBorderImage((*ObjectBrushList)[ObjectIndex]);
		return this;
	}

	//��������
	ShortcutContainer* SetObjectNum(int Num = 0)
	{
		ObjectNum = Num;
		//�������Ϊ0����1������ʾ
		if (ObjectNum == 0 || ObjectNum == 1)
			ObjectNumText->SetText(FString(""));
		else
			ObjectNumText->SetText(FString::FromInt(Num));

		return this;
	}
};

//��Դ����
namespace EResourceType
{
	enum Type
	{
		Plant = 0,
		Metal,
		Animal
	};
}

//��Դ���Խṹ��
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

		//������ԭʼ������������
		for (TArray<TArray<int>>::TIterator It(*FOI); It; ++It)
		{
			TArray<int> FlobObjectInfoItem;
			for (TArray<int>::TIterator Ih(*It); Ih; ++Ih)
				FlobObjectInfoItem.Add(*Ih);
			FlobObjectInfo.Add(FlobObjectInfoItem);
		}
	}
};

//Game�������
namespace EGameUIType
{
	enum Type
	{
		Game, //��ϷģʽUI
		Pause, //��ͣ
		Lose,	//���ˣ�����
		Package, //����
		ChatRoom, //������
	};
}

//����������
namespace EContainerType
{
	enum Type
	{
		Output, //�ϳɱ����
		Input,  //�ϳɱ�����
		Normal, //��ͨ����
		Shortcut, //���������
	};
}