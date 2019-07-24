// Fill out your copyright notice in the Description page of Project Settings.


#include "SSlAiShortcutWidget.h"
#include "SlateOptMacros.h"
#include "SlAiStyle.h"
#include "SlAiGameWidgetStyle.h"
#include "SBox.h"
#include "SOverlay.h"
#include "STextBlock.h"
#include "SUniformGridPanel.h"
#include "SBorder.h"


BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SSlAiShortcutWidget::Construct(const FArguments& InArgs)
{
	GameStyle = &SlAiStyle::Get().GetWidgetStyle<FSlAiGameStyle>("BPSlAiGameStyle");

	
	ChildSlot
	[
		SNew(SBox)
		.WidthOverride(900.f)
		.HeightOverride(160.f)
		[
			SNew(SOverlay)

			+SOverlay::Slot()
			.HAlign(HAlign_Center)
			.VAlign(VAlign_Top)
			[
				SAssignNew(ShortcutInfoTextBlock, STextBlock)
				.Font(GameStyle->Font_Outline_16)
				.ColorAndOpacity(GameStyle->FontColor_White)
			]

			+SOverlay::Slot()
			.HAlign(HAlign_Fill)
			.VAlign(VAlign_Fill)
			.Padding(FMargin(0.f, 60.f, 0.f, 0.f))
			[
				SAssignNew(GridPanel, SUniformGridPanel)
			]
		]
	];
	
	//
	IsInitializeContainer = false;
}
void SSlAiShortcutWidget::Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime)
{
	if (!IsInitializeContainer)
	{
		IsInitializeContainer = true;
		InitializeContainer();
	}
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION

void SSlAiShortcutWidget::InitializeContainer()
{
	for (int i = 0; i < 9; ++i)
	{
		//´´½¨ÈÝÆ÷
		TSharedPtr<SBorder> ContainerBorder;
		TSharedPtr<SBorder> ObjectImage;
		TSharedPtr<STextBlock> ObjectNumText;

		SAssignNew(ContainerBorder, SBorder)
		.Padding(FMargin(10.f))
			//´ýÉ¾³ý
		.BorderImage(&GameStyle->NormalContainerBrush)
		/////
		[
			SAssignNew(ObjectImage, SBorder)
			.HAlign(HAlign_Right)
			.VAlign(VAlign_Bottom)
			.Padding(FMargin(0.f, 0.f, 5.f, 0.f))
			//´ýÉ¾³ý
			.BorderImage(&GameStyle->EmptyBrush)
			/////
			[
				SAssignNew(ObjectNumText, STextBlock)
				.Font(GameStyle->Font_Outline_20)
				.ColorAndOpacity(GameStyle->FontColor_Black)
			//´ýÉ¾³ý
				.Text(FText::FromString("12"))
			/////
			]
		];

		GridPanel->AddSlot(i, 0)
		[
			ContainerBorder->AsShared()
		];
	}
}
