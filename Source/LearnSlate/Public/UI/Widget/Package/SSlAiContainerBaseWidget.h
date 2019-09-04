// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"
#include "SlAiTypes.h"

/**
 * 
 */
class LEARNSLATE_API SSlAiContainerBaseWidget : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SSlAiContainerBaseWidget)
	{}

	SLATE_ATTRIBUTE(int, WorkIndex)

	SLATE_END_ARGS()

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);

	//获取实例
	static TSharedPtr<SSlAiContainerBaseWidget> CreateContainer(EContainerType::Type NeedType, int WorkID);

protected:

	//容器各个组件
	TSharedPtr<class SBorder> ContainerBorder;
	TSharedPtr<class SBorder> ObjectImage;
	TSharedPtr<class STextBlock> ObjectNumText;

	//获取GameStyle
	const struct FSlAiGameStyle* GameStyle;

	//工作序号，用于标定在背包中的位置作用
	TAttribute<int> WorkIndex;
};
