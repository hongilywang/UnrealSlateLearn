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
	SLATE_END_ARGS()

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);

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

};
