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
