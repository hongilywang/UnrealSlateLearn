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
	//透明值
	float Alpha;
	//水平组件
	TSharedPtr<SHorizontalBox> CSBox;
	//名字
	TSharedPtr<STextBlock> CSName;
	//内容框
	TSharedPtr<SBorder> CSBorder;
	//内容
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

	//激活组件
	TSharedPtr<SHorizontalBox> ActiveItem(FText NewName, FText NewContent)
	{
		CSName->SetText(NewName);
		CSContent->SetText(NewContent);
		Alpha = 1.f;
		CSName->SetColorAndOpacity(FSlateColor(FLinearColor(0.f, 1.f, 0.f, Alpha)));
		CSBorder->SetColorAndOpacity(FLinearColor(0.f, 1.f, 0.f, Alpha));
		return CSBox;
	}

	//逐渐消失，返回是否已经未激活
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
	//获取GameStyle
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
	//临时序列
	TArray<TSharedPtr<ChatShowItem>> TempList;

	for (TArray<TSharedPtr<ChatShowItem>>::TIterator It(ActiveList); It; ++It)
	{
		//如果已经完成隐藏了
		if ((*It)->DeltaDisappear(InDeltaTime))
		{
			//从列表中删除这个信息
			ChatBox->RemoveSlot((*It)->CSBox->AsShared());
			//将这个信息添加到临时序列
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
	//如果未激活列表不为空
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
		//移除UI
		ChatBox->RemoveSlot(InsertItem->CSBox->AsShared());
	}

	//将这个信息激活并且添加到UI
	ChatBox->AddSlot()
		.HAlign(HAlign_Fill)
		.VAlign(VAlign_Fill)
		.FillHeight(1.f)
		[
			InsertItem->ActiveItem(NewName, NewContent)->AsShared()
		];
	//将信息插入激活的序列
	ActiveList.Push(InsertItem);
}

void SSlAiChatShowWidget::InitlizeItem()
{
	for (int i = 0; i < 10; ++i)
		UnActiveList.Add(MakeShareable(new ChatShowItem(&GameStyle->EmptyBrush, GameStyle->Font_16)));
}
