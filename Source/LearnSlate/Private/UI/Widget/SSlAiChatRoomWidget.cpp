// Fill out your copyright notice in the Description page of Project Settings.


#include "SSlAiChatRoomWidget.h"
#include "SlateOptMacros.h"
#include "SlAiStyle.h"
#include "SlAiGameWidgetStyle.h"
#include "SBox.h"
#include "SOverlay.h"
#include "SBoxPanel.h"
#include "SBorder.h"
#include "SMultiLineEditableText.h"
#include "SScrollBox.h"
#include "SEditableTextBox.h"
#include "SButton.h"
#include "STextBlock.h"

struct ChatMessageItem
{
	//ˮƽ���
	TSharedPtr<SHorizontalBox> CSBox;
	//����
	TSharedPtr<STextBlock> CSName;
	//���ݿ�
	TSharedPtr<SBorder> CSBorder;
	//����
	TSharedPtr<SMultiLineEditableText> CSContent;

	ChatMessageItem(const FSlateBrush* EmptyBrush, const FSlateFontInfo FontInfo)
	{
		SAssignNew(CSBox, SHorizontalBox)

			+ SHorizontalBox::Slot()
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
		return CSBox;
	}
};

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SSlAiChatRoomWidget::Construct(const FArguments& InArgs)
{
	//��ȡGameStyle
	GameStyle = &SlAiStyle::Get().GetWidgetStyle<FSlAiGameStyle>("BPSlAiGameStyle");

		ChildSlot
		[
			SNew(SBox)
			.WidthOverride(600.f)
			.HeightOverride(1080.f)
			[
				SNew(SBorder)
				.BorderImage(&GameStyle->ChatRoomBGBrush)
				[
					SNew(SOverlay)

					+ SOverlay::Slot()
					.HAlign(HAlign_Fill)
					.VAlign(VAlign_Bottom)
					.Padding(FMargin(0.f, 0.f, 0.f, 80.f))
					[
						SAssignNew(ScrollBox, SScrollBox)
					]

					+ SOverlay::Slot()
					.HAlign(HAlign_Fill)
					.VAlign(VAlign_Fill)
					.Padding(FMargin(0.f, 1000.f, 0.f, 0.f))
					[
						SNew(SOverlay)

						+ SOverlay::Slot()
						.HAlign(HAlign_Fill)
						.VAlign(VAlign_Fill)
						.Padding(FMargin(0.f, 0.f, 120.f, 0.f))
						[
							SAssignNew(EditTextBox, SEditableTextBox)
							.Font(GameStyle->Font_30)
							.OnTextCommitted(this, &SSlAiChatRoomWidget::SubmitEvent)
						]

						+SOverlay::Slot()
						.HAlign(HAlign_Fill)
						.VAlign(VAlign_Fill)
						.Padding(FMargin(480.f, 0.f, 0.f, 0.f))
						[
							SNew(SButton)
							.HAlign(HAlign_Center)
							.VAlign(VAlign_Center)
							.OnClicked(this, &SSlAiChatRoomWidget::SendEvent)
							[
								SNew(STextBlock)
								.Font(GameStyle->Font_30)
								.Text(NSLOCTEXT("SlAiGame", "Send", "Send"))
							]

						]
					]
				]
			]
	];
	
}
END_SLATE_FUNCTION_BUILD_OPTIMIZATION

void SSlAiChatRoomWidget::SubmitEvent(const FText& SubmitText, ETextCommit::Type CommitType)
{
	FString MessageStr = EditTextBox->GetText().ToString();
	if (MessageStr.IsEmpty())
		return;

	MessageStr = FString(": ") + MessageStr;
	AddMessage(NSLOCTEXT("SlAiGame", "Player", "Player"), FText::FromString(MessageStr));
	EditTextBox->SetText(FText::FromString(""));
	PushMessage.ExecuteIfBound(NSLOCTEXT("SlAiGame", "Player", "Player"), FText::FromString(MessageStr));
}

FReply SSlAiChatRoomWidget::SendEvent()
{
	FString MessageStr = EditTextBox->GetText().ToString();
	if (MessageStr.IsEmpty())
		return FReply::Handled();

	MessageStr = FString(": ") + MessageStr;
	AddMessage(NSLOCTEXT("SlAiGame", "Player", "Player"), FText::FromString(MessageStr));
	EditTextBox->SetText(FText::FromString(""));
	PushMessage.ExecuteIfBound(NSLOCTEXT("SlAiGame", "Player", "Player"), FText::FromString(MessageStr));
	return FReply::Handled();
}

void SSlAiChatRoomWidget::AddMessage(FText NewName, FText NewContent)
{
	TSharedPtr<ChatMessageItem> InsertItem;
	if (MessageList.Num() < 30)
	{
		//�´���һ���ؼ�
		InsertItem = MakeShareable(new ChatMessageItem(&GameStyle->EmptyBrush, GameStyle->Font_30));
		MessageList.Add(InsertItem);
		ScrollBox->AddSlot()
			[
				InsertItem->ActiveItem(NewName, NewContent)->AsShared()
			];
	}
	else
	{
		//������������ȡ
		InsertItem = MessageList[0];
		//������
		MessageList.Remove(InsertItem);
		ScrollBox->RemoveSlot(InsertItem->CSBox->AsShared());
		//�����
		ScrollBox->AddSlot()
			[
				InsertItem->ActiveItem(NewName, NewContent)->AsShared()
			];
		MessageList.Push(InsertItem);
	}
	//���ù�������ײ�
	ScrollBox->ScrollToEnd();
}

void SSlAiChatRoomWidget::ScrollToEnd()
{
	ScrollBox->ScrollToEnd();
}
