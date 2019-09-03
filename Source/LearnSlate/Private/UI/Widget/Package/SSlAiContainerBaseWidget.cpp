// Fill out your copyright notice in the Description page of Project Settings.


#include "SSlAiContainerBaseWidget.h"
#include "SlateOptMacros.h"
#include "SlAiStyle.h"
#include "SlAiGameWidgetStyle.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SSlAiContainerBaseWidget::Construct(const FArguments& InArgs)
{
	//获取GameStyle
	GameStyle = &SlAiStyle::Get().GetWidgetStyle<FSlAiGameStyle>("BPSlAiGameStyle");
	//获取工作号
	WorkIndex = InArgs._WorkIndex;

	ChildSlot
	[
		// Populate the widget
	];
	
}
END_SLATE_FUNCTION_BUILD_OPTIMIZATION
