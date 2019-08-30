// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"

/**
 * 
 */
class LEARNSLATE_API SSlAiPlayerStateWidget : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SSlAiPlayerStateWidget)
	{}
	SLATE_END_ARGS()

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);

	//更新状态事件
	void UpdateStateWidget(float HPValue, float HungerValue);

private:
	//获取GameStyle
	const struct FSlAiGameStyle* GameStyle;

	//血条
	TSharedPtr<class SProgressBar> HPBar;

	//饥饿度
	TSharedPtr<class SProgressBar> HungerBar;
};
