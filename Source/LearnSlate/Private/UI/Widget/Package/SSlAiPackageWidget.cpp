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
	//��ȡGameStyle
	GameStyle = &SlAiStyle::Get().GetWidgetStyle<FSlAiGameStyle>("BPSlAiGameStyle");

	ChildSlot
	[
		SNew(SOverlay)

		//����
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

				//����ͼ
				+SOverlay::Slot()
				.HAlign(HAlign_Fill)
				.VAlign(VAlign_Fill)
				[
					SNew(SImage)
					.Image(&GameStyle->PackageBGBrush)
				]

				//�ײ������
				+SOverlay::Slot()
				.HAlign(HAlign_Fill)
				.VAlign(VAlign_Fill)
				.Padding(FMargin(40.f, 680.f, 40.f, 40.f))
				[
					SAssignNew(ShortcutGrid, SUniformGridPanel)
				]

				//��������
				+ SOverlay::Slot()
				.HAlign(HAlign_Fill)
				.VAlign(VAlign_Fill)
				.Padding(FMargin(40.f, 320.f, 40.f, 160.f))
				[
					SAssignNew(PackageGrid, SUniformGridPanel)
				]

				//�ϳɱ������
				+ SOverlay::Slot()
				.HAlign(HAlign_Fill)
				.VAlign(VAlign_Fill)
				.Padding(FMargin(80.f, 40.f, 480.f, 520.f))
				[
					SAssignNew(CompoundGrid, SUniformGridPanel)
				]

				//�ϳɱ������
				+ SOverlay::Slot()
				.HAlign(HAlign_Fill)
				.VAlign(VAlign_Fill)
				.Padding(FMargin(560.f, 120.f, 160.f, 600.f))
				[
					SAssignNew(OutputBorder, SBorder)
				]

				//�ϳ�С��ͷ
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
	//��ʼ�������
	for (int i = 0; i < 9; ++i)
	{
		//��������ʵ��
		TSharedPtr<SSlAiContainerBaseWidget> NewContainer = SSlAiContainerBaseWidget::CreateContainer(EContainerType::Shortcut, i);
		//��������ӵ�UI
		ShortcutGrid->AddSlot(i, 0)[NewContainer->AsShared()];
	}

	//��ʼ������
	for (int i = 0; i < 36; ++i)
	{
		//��������ʵ��
		TSharedPtr<SSlAiContainerBaseWidget> NewContainer = SSlAiContainerBaseWidget::CreateContainer(EContainerType::Normal, i);
		//��������ӵ�UI
		PackageGrid->AddSlot(i % 9, i / 9)[NewContainer->AsShared()];
	}

	//��ʼ���ϳ�̨
	for (int i = 0; i < 9; ++i)
	{
		//��������ʵ��
		TSharedPtr<SSlAiContainerBaseWidget> NewContainer = SSlAiContainerBaseWidget::CreateContainer(EContainerType::Input, i);
		//��������ӵ�UI
		CompoundGrid->AddSlot(i % 3, i / 3)[NewContainer->AsShared()];
	}

	//��ʼ���������
	TSharedPtr<SSlAiContainerBaseWidget> NewContainer = SSlAiContainerBaseWidget::CreateContainer(EContainerType::Input, 1);
	OutputBorder->SetContent(NewContainer->AsShared());
}
