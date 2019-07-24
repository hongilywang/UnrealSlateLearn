// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"

/**
 * 
 */
class LEARNSLATE_API SSlAiGameHUDWidget : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SSlAiGameHUDWidget)
	{}
	SLATE_END_ARGS()

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);

	//绑定到UIScaler的方法
	float GetUIScaler() const;

public:

	//快捷栏指针
	TSharedPtr<class SSlAiShortcutWidget> ShortcutWidget;

private:
	//获取屏幕size
	FVector2D GetViewportSize() const;


private:

	//DPI缩放
	TAttribute<float> UIScaler;
};
