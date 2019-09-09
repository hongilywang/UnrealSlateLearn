// Fill out your copyright notice in the Description page of Project Settings.


#include "SSlAiContainerBaseWidget.h"
#include "SlateOptMacros.h"
#include "SlAiStyle.h"
#include "SlAiGameWidgetStyle.h"

#include "SBorder.h"
#include "STextBlock.h"

#include "SSlAiContainerInputWidget.h"
#include "SSlAiContainerNormalWidget.h"
#include "SSlAiContainerOutputWidget.h"
#include "SSlAiContainerShortcutWidget.h"
#include "SlAiDataHandle.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SSlAiContainerBaseWidget::Construct(const FArguments& InArgs)
{
	//获取GameStyle
	GameStyle = &SlAiStyle::Get().GetWidgetStyle<FSlAiGameStyle>("BPSlAiGameStyle");
	//获取工作号
	WorkIndex = InArgs._WorkIndex;

	ChildSlot
	[
		SAssignNew(ContainerBorder, SBorder)
		.BorderImage(&GameStyle->NormalContainerBrush)
		.Padding(FMargin(8.f))
		[
			SAssignNew(ObjectImage, SBorder)
			.BorderImage(&GameStyle->EmptyBrush)
			.HAlign(HAlign_Right)
			.VAlign(VAlign_Bottom)
			.Padding(FMargin(0.f, 0.f, 4.f, 0.f))
			[
				SAssignNew(ObjectNumText, STextBlock)
				.Font(GameStyle->Font_Outline_16)
				.ColorAndOpacity(GameStyle->FontColor_Black)
			]
		]
	];
	IsHover = false;
	ObjectIndex = 0;
	ObjectNum = 0;
}
END_SLATE_FUNCTION_BUILD_OPTIMIZATION

TSharedPtr<SSlAiContainerBaseWidget> SSlAiContainerBaseWidget::CreateContainer(EContainerType::Type NeedType, int WorkID)
{
	TSharedPtr<SSlAiContainerBaseWidget> ResultContainer;
	switch (NeedType)
	{
	case EContainerType::Output:
		SAssignNew(ResultContainer, SSlAiContainerOutputWidget).WorkIndex(WorkID);
		break;
	case EContainerType::Input:
		SAssignNew(ResultContainer, SSlAiContainerInputWidget).WorkIndex(WorkID);
		break;
	case EContainerType::Normal:
		SAssignNew(ResultContainer, SSlAiContainerNormalWidget).WorkIndex(WorkID);
		break;
	case EContainerType::Shortcut:
		SAssignNew(ResultContainer, SSlAiContainerShortcutWidget).WorkIndex(WorkID);
		break;
	}

	return ResultContainer;
}

void SSlAiContainerBaseWidget::UpdateHovered(bool IsHovered)
{
	if (IsHovered == IsHover)
		return;

	if (IsHovered)
		ContainerBorder->SetBorderImage(&GameStyle->ChoosedContainerBrush);
	else
		ContainerBorder->SetBorderImage(&GameStyle->NormalContainerBrush);

	IsHover = IsHovered;
}

void SSlAiContainerBaseWidget::ResetContainerPara(int ObjectID, int Num)
{
	//如果输入ID不同，更新贴图
	if (ObjectIndex != ObjectID)
		ObjectImage->SetBorderImage(SlAiDataHandle::Get()->ObjectBrushList[ObjectID]);
	ObjectIndex = ObjectID;
	ObjectNum = Num;

	//如果物品ID为0
	if (ObjectNum == 0)
	{
		ObjectNumText->SetText(FText::FromString(""));
	}
	else
	{
		//判断物品是否可以叠加，是的话显示数量
		if (MultiplyAble(ObjectIndex))
			ObjectNumText->SetText(FText::FromString(FString::FromInt(ObjectNum)));
		else
			ObjectNumText->SetText(FText::FromString(""));
	}
}

int SSlAiContainerBaseWidget::GetIndex() const
{
	return ObjectIndex;
}

int SSlAiContainerBaseWidget::GetNum() const
{
	return ObjectNum;
}

void SSlAiContainerBaseWidget::LeftOperate(int InputID, int InputNum, int& OutputID, int& OutputNum)
{
	//如果输入物品与本地物品相同并且可以叠加
	if (InputID == ObjectIndex && MultiplyAble(ObjectIndex))
	{
		//可叠加的上限是64
		OutputID = (ObjectNum + InputNum) <= 64 ? 0 : InputID;
		OutputNum = (OutputNum + InputNum) <= 64 ? 0 : (ObjectNum + InputNum - 64);
		ObjectNum = (ObjectNum + InputNum) <= 64 ? (ObjectNum + InputNum) : 64;
		//更新属性
		ResetContainerPara(ObjectIndex, ObjectNum);
	}
}

void SSlAiContainerBaseWidget::RightOperate(int InputID, int InputNum, int& OutputID, int& OutputNum)
{

}

bool SSlAiContainerBaseWidget::MultiplyAble(int ObjectID)
{
	//获取物品属性
	TSharedPtr<ObjectAttribute> ObjectAttr = *SlAiDataHandle::Get()->ObjectAttrMap.Find(ObjectID);
	//返回是否是武器或者工具
	return (ObjectAttr->ObjectType != EObjectType::Tool && ObjectAttr->ObjectType != EObjectType::Weapon);
}
