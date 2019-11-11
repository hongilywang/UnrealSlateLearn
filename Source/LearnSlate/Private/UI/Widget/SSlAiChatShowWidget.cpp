// Fill out your copyright notice in the Description page of Project Settings.


#include "SSlAiChatShowWidget.h"
#include "SlateOptMacros.h"
#include "SlAiStyle.h"
#include "SlAiGameWidgetStyle.h"
#include "SBox.h"
#include "SOverlay.h"
#include "SBoxPanel.h"
#include "SBorder.h"
#include "STextBlock.h"
#include "SMultiLineEditableText.h"

struct ChatShowItem
{
	//͸��ֵ
	float Alpha;
	//ˮƽ���
	TSharedPtr<SHorizontalBox> CSBox;
	//����
	TSharedPtr<STextBlock> CSName;
	//���ݿ�
	TSharedPtr<SBorder> CSBorder;
	//����
	TSharedPtr<SMultiLineEditableText> CSContent;

	ChatShowItem(const FSlateBrush* EmptyBrush, const FSlateFontInfo FontInfo)
	{
		Alpha = 0.f;
		SAssignNew(CSBox, SHorizontalBox)

			+SHorizontalBox::Slot()
			.HAlign(HAlign_Fill)
			.VAlign(VAlign_Fill)
			.AutoWidth()
			[
				SAssignNew(CSName, STextBlock)
				.Font(FontInfo)
				.ColorAndOpacity(FSlateColor(FLinearColor::Green))
			]

			+ SHorizontalBox::Slot()
			.HAlign(HAlign_Fill)
			.VAlign(VAlign_Fill)
			.FillWidth(1.f)
			[
				SAssignNew(CSBorder, SBorder)
				.BorderImage(EmptyBrush)
				.HAlign(HAlign_Fill)
				.VAlign(VAlign_Fill)
				[
					SAssignNew(CSContent, SMultiLineEditableText)
					.WrappingPolicy(ETextWrappingPolicy::AllowPerCharacterWrapping)
					.AutoWrapText(true)
					.Font(FontInfo)
				]
			];
	}

	//�������
	TSharedPtr<SHorizontalBox> ActiveItem(FText NewName, FText NewContent)
	{
		CSName->SetText(NewName);
		CSContent->SetText(NewContent);
		Alpha = 1.f;
		CSName->SetColorAndOpacity(FSlateColor(FLinearColor(0.f, 1.f, 0.f, Alpha)));
		CSBorder->SetColorAndOpacity(FLinearColor(0.f, 1.f, 0.f, Alpha));
		return CSBox;
	}

	//����ʧ�������Ƿ��Ѿ�δ����
	bool DeltaDisappear(float DeltaTime)
	{
		Alpha = FMath::Clamp<float>(Alpha - DeltaTime * 0.05f, 0.f, 1.f);
		CSName->SetColorAndOpacity(FSlateColor(FLinearColor(0.f, 1.f, 0.f, Alpha)));
		CSBorder->SetColorAndOpacity(FLinearColor(0.f, 1.f, 0.f, Alpha));
		if (Alpha == 0.5f)
			return true;

		return false;
	}
};

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SSlAiChatShowWidget::Construct(const FArguments& InArgs)
{
	//��ȡGameStyle
	GameStyle = &SlAiStyle::Get().GetWidgetStyle<FSlAiGameStyle>("BPSlAiGameStyle");

	
	ChildSlot
	[
		SNew(SBox)
		.WidthOverride(500.f)
		.HeightOverride(600.f)
		.HAlign(HAlign_Fill)
		.VAlign(VAlign_Bottom)
		[
			SAssignNew(ChatBox, SVerticalBox)
		]
	];

	InitlizeItem();
	
}
END_SLATE_FUNCTION_BUILD_OPTIMIZATION

void SSlAiChatShowWidget::Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime)
{
	//��ʱ����
	TArray<TSharedPtr<ChatShowItem>> TempList;

	for (TArray<TSharedPtr<ChatShowItem>>::TIterator It(ActiveList); It; ++It)
	{
		//����Ѿ����������
		if ((*It)->DeltaDisappear(InDeltaTime))
		{
			//���б���ɾ�������Ϣ
			ChatBox->RemoveSlot((*It)->CSBox->AsShared());
			//�������Ϣ��ӵ���ʱ����
			TempList.Push(*It);
		}
	}

	for (int i = 0; i < TempList.Num(); ++i)
	{
		ActiveList.Remove(TempList[i]);
		UnActiveList.Push(TempList[i]);
	}
}

void SSlAiChatShowWidget::AddMessage(FText NewName, FText NewContent)
{
	TSharedPtr<ChatShowItem> InsertItem;
	//���δ�����б�Ϊ��
	if (UnActiveList.Num() > 0)
	{
		//
		InsertItem = UnActiveList[0];
		UnActiveList.RemoveAt(0);
	}
	else
	{
		InsertItem = ActiveList[0];
		ActiveList.RemoveAt(0);
		//�Ƴ�UI
		ChatBox->RemoveSlot(InsertItem->CSBox->AsShared());
	}

	//�������Ϣ�������ӵ�UI
	ChatBox->AddSlot()
		.HAlign(HAlign_Fill)
		.VAlign(VAlign_Fill)
		.FillHeight(1.f)
		[
			InsertItem->ActiveItem(NewName, NewContent)->AsShared()
		];
	//����Ϣ���뼤�������
	ActiveList.Push(InsertItem);
}

void SSlAiChatShowWidget::InitlizeItem()
{
	for (int i = 0; i < 10; ++i)
		UnActiveList.Add(MakeShareable(new ChatShowItem(&GameStyle->EmptyBrush, GameStyle->Font_16)));
}
