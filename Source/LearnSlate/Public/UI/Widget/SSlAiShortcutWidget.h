// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"

/**
 * 
 */
class LEARNSLATE_API SSlAiShortcutWidget : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SSlAiShortcutWidget)
	{}
	SLATE_END_ARGS()

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);

	virtual void Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime) override;

private:
	//��ʼ����������
	void InitializeContainer();

private:

	//��ȡGameStyle
	const struct FSlAiGameStyle* GameStyle;

	//��Ʒ��Ϣָ��
	TSharedPtr<class STextBlock> ShortcutInfoTextBlock;

	//����ָ��
	TSharedPtr<class SUniformGridPanel> GridPanel;

	//�Ƿ��ʼ������
	bool IsInitializeContainer;
};
