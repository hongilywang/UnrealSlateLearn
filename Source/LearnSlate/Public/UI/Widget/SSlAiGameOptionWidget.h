// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SlAiTypes.h"
#include "Widgets/SCompoundWidget.h"

class SCheckBox;
class SSlider;
class STextBlock;
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
	//音量变化事件
	void MusicSliderChanged(float value);
	//音效变化事件
	void SoundSliderChanged(float value);

private:
	const struct FSlAiMenuStyle* MenuStyle;
	//获取CheckBox指针
	TSharedPtr<SCheckBox> EnCheckBox;
	TSharedPtr<SCheckBox> ZhCheckBox;
	//两个进度条
	TSharedPtr<SSlider> MuSlider; //背景音乐
	TSharedPtr<SSlider> SoSlider; //音效
	//进度条百分百
	TSharedPtr<STextBlock> MuTexBlock;
	//进度条百分百
	TSharedPtr<STextBlock> SoTexBlock;
};
