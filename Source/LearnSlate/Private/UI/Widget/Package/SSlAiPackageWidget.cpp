// Fill out your copyright notice in the Description page of Project Settings.


#include "SSlAiPackageWidget.h"
#include "SlateOptMacros.h"
#include "SlAiStyle.h"
#include "SlAiGameWidgetStyle.h"

#include "SBox.h"
#include "STextBlock.h"
#include "SImage.h"
#include "SSlAiContainerBaseWidget.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SSlAiPackageWidget::Construct(const FArguments& InArgs)
{
	//获取GameStyle
	GameStyle = &SlAiStyle::Get().GetWidgetStyle<FSlAiGameStyle>("BPSlAiGameStyle");

	ChildSlot
	[
		SNew(SOverlay)

		//背包
		+SOverlay::Slot()
		.HAlign(HAlign_Right)
		.VAlign(VAlign_Center)
		.Padding(FMargin(0.f, 0.f, 50.f, 0.f))
		[
			SNew(SBox)
			.WidthOverride(800.f)
			.HeightOverride(800.f)
			[
				SNew(SOverlay)

				//背包图
				+SOverlay::Slot()
				.HAlign(HAlign_Fill)
				.VAlign(VAlign_Fill)
				[
					SNew(SImage)
					.Image(&GameStyle->PackageBGBrush)
				]

				//底部快捷栏
				+SOverlay::Slot()
				.HAlign(HAlign_Fill)
				.VAlign(VAlign_Fill)
				.Padding(FMargin(40.f, 680.f, 40.f, 40.f))
				[
					SAssignNew(ShortcutGrid, SUniformGridPanel)
				]

				//背包主体
				+ SOverlay::Slot()
				.HAlign(HAlign_Fill)
				.VAlign(VAlign_Fill)
				.Padding(FMargin(40.f, 320.f, 40.f, 160.f))
				[
					SAssignNew(PackageGrid, SUniformGridPanel)
				]

				//合成表输入框
				+ SOverlay::Slot()
				.HAlign(HAlign_Fill)
				.VAlign(VAlign_Fill)
				.Padding(FMargin(80.f, 40.f, 480.f, 520.f))
				[
					SAssignNew(CompoundGrid, SUniformGridPanel)
				]

				//合成表输出框
				+ SOverlay::Slot()
				.HAlign(HAlign_Fill)
				.VAlign(VAlign_Fill)
				.Padding(FMargin(560.f, 120.f, 160.f, 600.f))
				[
					SAssignNew(OutputBorder, SBorder)
				]

				//合成小箭头
				+ SOverlay::Slot()
				.HAlign(HAlign_Fill)
				.VAlign(VAlign_Fill)
				.Padding(FMargin(400.f, 120.f, 320.f, 600.f))
				[
					SNew(SImage)
					.Image(&GameStyle->CompoundArrowBrush)
				]
			]
		]
	];
	
}
END_SLATE_FUNCTION_BUILD_OPTIMIZATION

void SSlAiPackageWidget::InitPackageManager()
{
	//初始化快捷栏
	for (int i = 0; i < 9; ++i)
	{
		//创建容器实例
		TSharedPtr<SSlAiContainerBaseWidget> NewContainer = SSlAiContainerBaseWidget::CreateContainer(EContainerType::Shortcut, i);
		//将容器添加到UI
		ShortcutGrid->AddSlot(i, 0)[NewContainer->AsShared()];
	}

	//初始化背包
	for (int i = 0; i < 36; ++i)
	{
		//创建容器实例
		TSharedPtr<SSlAiContainerBaseWidget> NewContainer = SSlAiContainerBaseWidget::CreateContainer(EContainerType::Normal, i);
		//将容器添加到UI
		PackageGrid->AddSlot(i % 9, i / 9)[NewContainer->AsShared()];
	}

	//初始化合成台
	for (int i = 0; i < 9; ++i)
	{
		//创建容器实例
		TSharedPtr<SSlAiContainerBaseWidget> NewContainer = SSlAiContainerBaseWidget::CreateContainer(EContainerType::Input, i);
		//将容器添加到UI
		CompoundGrid->AddSlot(i % 3, i / 3)[NewContainer->AsShared()];
	}

	//初始化输出容器
	TSharedPtr<SSlAiContainerBaseWidget> NewContainer = SSlAiContainerBaseWidget::CreateContainer(EContainerType::Input, 1);
	OutputBorder->SetContent(NewContainer->AsShared());
}
