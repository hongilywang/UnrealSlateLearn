// Fill out your copyright notice in the Description page of Project Settings.


#include "SSlAiGameOptionWidget.h"
#include "SlateOptMacros.h"
#include "SlAiStyle.h"
#include "SlAiMenuWidgetStyle.h"
#include "SBox.h"
#include "SImage.h"
#include "SBorder.h"
#include "SOverlay.h"
#include "STextBlock.h"
#include "SBoxPanel.h"
#include "SCheckBox.h"

#include "SlAiDataHandle.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SSlAiGameOptionWidget::Construct(const FArguments& InArgs)
{
	MenuStyle = &SlAiStyle::Get().GetWidgetStyle<FSlAiMenuStyle>("BPSlAiMenuStyle");

	
	ChildSlot
	[
		SNew(SBox)
		.WidthOverride(500.f)
		.HeightOverride(300.f)
		[
			SNew(SOverlay)

			+SOverlay::Slot()
			.HAlign(HAlign_Fill)
			.VAlign(VAlign_Fill)
			[
				SNew(SImage)
				.Image(&MenuStyle->GameOptionBrush)
			]

			+ SOverlay::Slot()
			.HAlign(HAlign_Fill)
			.VAlign(VAlign_Fill)
			.Padding(FMargin(50.f))
			[
				SNew(SVerticalBox)
				//��һ��
				+ SVerticalBox::Slot()
				.VAlign(VAlign_Fill)
				.HAlign(HAlign_Fill)
				.FillHeight(1.f)
				[
					SNew(SHorizontalBox)

					+ SHorizontalBox::Slot()
					.HAlign(HAlign_Center)
					.VAlign(VAlign_Center)
					.FillWidth(1.f)
					[
						SAssignNew(ZhCheckBox, SCheckBox)
						.OnCheckStateChanged(this, &SSlAiGameOptionWidget::ZhCheckBoxStateChanged)
						[
							SNew(STextBlock)
							.Font(MenuStyle->Font_40)
							.ColorAndOpacity(MenuStyle->FontColor_Black)
							.Text(NSLOCTEXT("SlAiMenu", "Chinese", "Chinese"))
						]
					]

					+ SHorizontalBox::Slot()
					.HAlign(HAlign_Center)
					.VAlign(VAlign_Center)
					.FillWidth(1.f)
					[
						SAssignNew(EnCheckBox, SCheckBox)
						.OnCheckStateChanged(this, &SSlAiGameOptionWidget::EnCheckBoxStateChanged)
						[
							SNew(STextBlock)
							.Font(MenuStyle->Font_40)
							.ColorAndOpacity(MenuStyle->FontColor_Black)
							.Text(NSLOCTEXT("SlAiMenu", "English", "English"))
						]
					]
				]

				//��2��
				+ SVerticalBox::Slot()
				.VAlign(VAlign_Fill)
				.HAlign(HAlign_Fill)
				.FillHeight(1.f)
				[
					SNew(SOverlay)
				]
				//��3��
				+ SVerticalBox::Slot()
				.VAlign(VAlign_Fill)
				.HAlign(HAlign_Fill)
				.FillHeight(1.f)
				[
					SNew(SOverlay)
				]
]
		]
	];
	
	StyleInitialize();
}

void SSlAiGameOptionWidget::StyleInitialize()
{
	//����ZhCheckBox��ʽ
	ZhCheckBox->SetUncheckedImage(&MenuStyle->UnCheckedBoxBrush);
	ZhCheckBox->SetUncheckedHoveredImage(&MenuStyle->UnCheckedBoxBrush);
	ZhCheckBox->SetUncheckedPressedImage(&MenuStyle->UnCheckedBoxBrush);
	ZhCheckBox->SetCheckedImage(&MenuStyle->CheckedBoxBrush);
	ZhCheckBox->SetCheckedHoveredImage(&MenuStyle->CheckedBoxBrush);
	ZhCheckBox->SetCheckedPressedImage(&MenuStyle->CheckedBoxBrush);

	//����EnCheckBox��ʽ
	EnCheckBox->SetUncheckedImage(&MenuStyle->UnCheckedBoxBrush);
	EnCheckBox->SetUncheckedHoveredImage(&MenuStyle->UnCheckedBoxBrush);
	EnCheckBox->SetUncheckedPressedImage(&MenuStyle->UnCheckedBoxBrush);
	EnCheckBox->SetCheckedImage(&MenuStyle->CheckedBoxBrush);
	EnCheckBox->SetCheckedHoveredImage(&MenuStyle->CheckedBoxBrush);
	EnCheckBox->SetCheckedPressedImage(&MenuStyle->CheckedBoxBrush);

	switch (SlAiDataHandle::Get()->CurrentCulture)
	{
	case ECultureTeam::EN:
		EnCheckBox->SetIsChecked(ECheckBoxState::Checked);
		ZhCheckBox->SetIsChecked(ECheckBoxState::Unchecked);
		break;
	case ECultureTeam::ZH:
		ZhCheckBox->SetIsChecked(ECheckBoxState::Checked);
		EnCheckBox->SetIsChecked(ECheckBoxState::Unchecked);
		break;
	}
}

void SSlAiGameOptionWidget::ZhCheckBoxStateChanged(ECheckBoxState NewState)
{
	//���ñ�ѡ�еĿ�
	ZhCheckBox->SetIsChecked(ECheckBoxState::Checked);
	EnCheckBox->SetIsChecked(ECheckBoxState::Unchecked);
	//�������ݿ����໻Ϊ����
	SlAiDataHandle::Get()->ChangeLocalizationCulture(ECultureTeam::ZH);
}

void SSlAiGameOptionWidget::EnCheckBoxStateChanged(ECheckBoxState NewState)
{
	//���ñ�ѡ�еĿ�
	ZhCheckBox->SetIsChecked(ECheckBoxState::Unchecked);
	EnCheckBox->SetIsChecked(ECheckBoxState::Checked);
	//�������ݿ����໻ΪӢ��
	SlAiDataHandle::Get()->ChangeLocalizationCulture(ECultureTeam::EN);
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION
