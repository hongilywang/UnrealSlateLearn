// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SlAiTypes.h"
#include "Widgets/SCompoundWidget.h"

class SCheckBox;
/**
 * 
 */
class LEARNSLATE_API SSlAiGameOptionWidget : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SSlAiGameOptionWidget)
	{}
	SLATE_END_ARGS()

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);

private:

	//统一设置样式
	void StyleInitialize();

	//中文checkbox事件
	void ZhCheckBoxStateChanged(ECheckBoxState NewState);
	//英文checkbox事件
	void EnCheckBoxStateChanged(ECheckBoxState NewState);

private:
	const struct FSlAiMenuStyle* MenuStyle;
	//获取CheckBox指针
	TSharedPtr<SCheckBox> EnCheckBox;
	TSharedPtr<SCheckBox> ZhCheckBox;
};
