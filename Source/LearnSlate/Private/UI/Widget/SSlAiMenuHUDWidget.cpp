// Fill out your copyright notice in the Description page of Project Settings.


#include "SSlAiMenuHUDWidget.h"
#include "SlateOptMacros.h"
#include "SButton.h"
#include "SImage.h"
#include "SlAiStyle.h"
#include "SlAiMenuWidgetStyle.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SSlAiMenuHUDWidget::Construct(const FArguments& InArgs)
{
	//»ñÈ¡±à¼­Æ÷µÄMenuStyle
	MenuStyle = &SlAiStyle::Get().GetWidgetStyle<FSlAiMenuStyle>("BPSlAiMenuStyle");

	ChildSlot
	[
		SNew(SImage)
		.Image(&MenuStyle->MenuBackgroundBrush)
	];
	
}
END_SLATE_FUNCTION_BUILD_OPTIMIZATION
