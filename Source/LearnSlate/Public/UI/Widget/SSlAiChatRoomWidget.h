// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"

struct ChatMessageItem;

DECLARE_DELEGATE_TwoParams(FPushMessage, FText, FText)
/**
 * 
 */
class LEARNSLATE_API SSlAiChatRoomWidget : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SSlAiChatRoomWidget)
	{}
	SLATE_END_ARGS()

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);

	//提交事件
	void SubmitEvent(const FText& SubmitText, ETextCommit::Type CommitType);

	//按钮事件
	FReply SendEvent();

	//添加信息
	void AddMessage(FText NewName, FText NewContent);

	//滚动到最底部
	void ScrollToEnd();

public:
	FPushMessage PushMessage;

private:
	const struct FSlAiGameStyle* GameStyle;

	//滚动框
	TSharedPtr<class SScrollBox> ScrollBox;

	//输入框
	TSharedPtr<class SEditableTextBox> EditTextBox;

	//保存输出
	TArray<TSharedPtr<ChatMessageItem>> MessageList;
};
