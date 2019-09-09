// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"

/**
 * 
 */
class LEARNSLATE_API SSlAiPackageWidget : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SSlAiPackageWidget)
	{}
	SLATE_ATTRIBUTE(float, UIScaler)
	SLATE_END_ARGS()

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);

	virtual void Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime) override;

	//重写绘制函数
	virtual int32 OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const override;

	//重写鼠标点击事件
	virtual FReply OnMouseButtonDown(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;

	//注册背包管理器事件，由playercharacter的InitPackageManager委托进行调用
	void InitPackageManager();

private:
	//获取GameStyle
	const struct FSlAiGameStyle* GameStyle;

	//快捷栏表格
	TSharedPtr<class SUniformGridPanel> ShortcutGrid;
	//背包控件
	TSharedPtr<class SUniformGridPanel> PackageGrid;
	//合成表表格
	TSharedPtr<class SUniformGridPanel> CompoundGrid;
	//输出容器
	TSharedPtr<class SBorder> OutputBorder;

	//鼠标位置
	FVector2D MousePosition;

	//DPI缩放
	TAttribute<float> UIScaler;

	//是否已经初始化背包管理器
	bool IsInitPackageMana;
};
