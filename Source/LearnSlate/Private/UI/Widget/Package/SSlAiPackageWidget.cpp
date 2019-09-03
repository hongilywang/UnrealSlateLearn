// Fill out your copyright notice in the Description page of Project Settings.


#include "SSlAiPackageWidget.h"
#include "SlateOptMacros.h"

#include "SBox.h"
#include "STextBlock.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SSlAiPackageWidget::Construct(const FArguments& InArgs)
{
	
	ChildSlot
	[
		SNew(SBox)
		.WidthOverride(400.f)
		.HeightOverride(100.f)
		[
			SNew(STextBlock)
			.Text(FText::FromString("Package"))
		]
	];
	
}
END_SLATE_FUNCTION_BUILD_OPTIMIZATION
