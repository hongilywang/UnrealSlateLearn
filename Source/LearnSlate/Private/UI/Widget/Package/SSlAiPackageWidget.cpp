// Fill out your copyright notice in the Description page of Project Settings.


#include "SSlAiPackageWidget.h"
#include "SlateOptMacros.h"
#include "SlAiStyle.h"
#include "SlAiGameWidgetStyle.h"

#include "SBox.h"
#include "STextBlock.h"
#include "SImage.h"
#include "SSlAiContainerBaseWidget.h"
#include "Engine/Engine.h"
#include "SlAiHelper.h"

#include "SlAiPackageManager.h"
#include "DrawElements.h"
#include "SlAiDataHandle.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SSlAiPackageWidget::Construct(const FArguments& InArgs)
{
	//��ȡGameStyle
	GameStyle = &SlAiStyle::Get().GetWidgetStyle<FSlAiGameStyle>("BPSlAiGameStyle");
	UIScaler = InArgs._UIScaler;

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
	
	MousePosition = FVector2D(0.f, 0.f);
	IsInitPackageMana = false;
}

void SSlAiPackageWidget::Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime)
{
	//���������ʾ����ʵ�ʴ��ڣ�ʵʱ�������λ��
	if (GetVisibility() == EVisibility::Visible && GEngine)
	{
		GEngine->GameViewport->GetMousePosition(MousePosition);
		//SlAiHelper::Debug(FString("AbsoMousePos : ") + MousePosition.ToString(), 0.f);
		MousePosition = MousePosition / UIScaler.Get();
		//SlAiHelper::Debug(FString("RelaMousePos : ") + MousePosition.ToString(), 0.f);
	}

	//��������������Ѿ���ʼ��
	if (IsInitPackageMana)
	{
		SlAiPackageManager::Get()->UpdateHovered(MousePosition, AllottedGeometry);
	}
}
END_SLATE_FUNCTION_BUILD_OPTIMIZATION

int32 SSlAiPackageWidget::OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const
{
	//���ø���ĺ���
	SCompoundWidget::OnPaint(Args, AllottedGeometry, MyCullingRect, OutDrawElements, LayerId, InWidgetStyle, bParentEnabled);

	//�����������û�г�ʼ��
	if (!IsInitPackageMana)
		return LayerId;

	//���������������������Ʒ��Ϊ0�� �ͽ�����Ⱦ
	if (GetVisibility() == EVisibility::Visible && SlAiPackageManager::Get()->ObjectIndex != 0 && SlAiPackageManager::Get()->ObjectNum != 0)
	{
		//��Ⱦ��Ʒͼ��
		FSlateDrawElement::MakeBox(OutDrawElements, LayerId + 30, AllottedGeometry.ToPaintGeometry(MousePosition - FVector2D(32.f, 32.f), FVector2D(64.f, 64.f)), SlAiDataHandle::Get()->ObjectBrushList[SlAiPackageManager::Get()->ObjectIndex], ESlateDrawEffect::None, FLinearColor(1.f, 1.f, 1.f));
		
		//��ȡ��Ʒ����
		TSharedPtr<ObjectAttribute> ObjectAttr = *SlAiDataHandle::Get()->ObjectAttrMap.Find(SlAiPackageManager::Get()->ObjectIndex);
		//��Ⱦ������������������Ե�����Ʒ�Ͳ���Ⱦ
		if (ObjectAttr->ObjectType != EObjectType::Tool && ObjectAttr->ObjectType != EObjectType::Type::Weapon)
		{
			//��Ⱦ��������
			FSlateDrawElement::MakeText(OutDrawElements, LayerId + 30, AllottedGeometry.ToPaintGeometry(MousePosition + FVector2D(12.f, 16.f), FVector2D(16.f, 16.f)), FString::FromInt(SlAiPackageManager::Get()->ObjectNum), GameStyle->Font_Outline_16, ESlateDrawEffect::None, GameStyle->FontColor_Black);
		}
	}

	return LayerId;
}

FReply SSlAiPackageWidget::OnMouseButtonDown(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	//������������ʼ��
	if (!IsInitPackageMana)
		return FReply::Handled();

	//�����������
	if (MouseEvent.IsMouseButtonDown(EKeys::LeftMouseButton))
	{
		SlAiPackageManager::Get()->LeftOption(MousePosition, MyGeometry);
	}
	//������Ҽ����
	else if (MouseEvent.IsMouseButtonDown(EKeys::RightMouseButton))
	{
		SlAiPackageManager::Get()->RightOption(MousePosition, MyGeometry);
	}

	return FReply::Handled();
}

void SSlAiPackageWidget::InitPackageManager()
{
	//��ʼ�������
	for (int i = 0; i < 9; ++i)
	{
		//��������ʵ��
		TSharedPtr<SSlAiContainerBaseWidget> NewContainer = SSlAiContainerBaseWidget::CreateContainer(EContainerType::Shortcut, i);
		//��������ӵ�UI
		ShortcutGrid->AddSlot(i, 0)[NewContainer->AsShared()];
		//ע������������������
		SlAiPackageManager::Get()->InsertContainer(NewContainer, EContainerType::Shortcut);
	}

	//��ʼ������
	for (int i = 0; i < 36; ++i)
	{
		//��������ʵ��
		TSharedPtr<SSlAiContainerBaseWidget> NewContainer = SSlAiContainerBaseWidget::CreateContainer(EContainerType::Normal, i);
		//��������ӵ�UI
		PackageGrid->AddSlot(i % 9, i / 9)[NewContainer->AsShared()];
		//ע������������������
		SlAiPackageManager::Get()->InsertContainer(NewContainer, EContainerType::Normal);
	}

	//��ʼ���ϳ�̨
	for (int i = 0; i < 9; ++i)
	{
		//��������ʵ��
		TSharedPtr<SSlAiContainerBaseWidget> NewContainer = SSlAiContainerBaseWidget::CreateContainer(EContainerType::Input, i);
		//��������ӵ�UI
		CompoundGrid->AddSlot(i % 3, i / 3)[NewContainer->AsShared()];
		//ע������������������
		SlAiPackageManager::Get()->InsertContainer(NewContainer, EContainerType::Input);
	}

	//��ʼ���������
	TSharedPtr<SSlAiContainerBaseWidget> NewContainer = SSlAiContainerBaseWidget::CreateContainer(EContainerType::Output, 1);
	OutputBorder->SetContent(NewContainer->AsShared());
	SlAiPackageManager::Get()->InsertContainer(NewContainer, EContainerType::Output);

	IsInitPackageMana = true;
}
