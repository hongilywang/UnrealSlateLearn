// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SlAiTypes.h"
#include "Widgets/SCompoundWidget.h"

class SCheckBox;
class SSlider;
class STextBlock;

//修改中英文委托
DECLARE_DELEGATE_OneParam(FChangeCulture, const ECultureTeam)
//修改音量委托
DECLARE_DELEGATE_TwoParams(FChangeVolume, const float, const float)

/**
 * 
 */
class LEARNSLATE_API SSlAiGameOptionWidget : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SSlAiGameOptionWidget)
	{}
	SLATE_EVENT(FChangeCulture, ChangeCulture)
	SLATE_EVENT(FChangeVolume, ChangeVolume)
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

	//委托变了
	FChangeCulture ChangeCulture;
	FChangeVolume ChangeVolume;
};
