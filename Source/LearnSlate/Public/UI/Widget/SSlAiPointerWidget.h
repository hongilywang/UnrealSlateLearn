// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"

/**
 * 
 */
class LEARNSLATE_API SSlAiPointerWidget : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SSlAiPointerWidget)
	{}
	SLATE_END_ARGS()

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);

	virtual void Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime) override;

	//给PlayerController绑定的事件，修改准星是否锁定已经加载进度
	void UpdatePointer(bool IsAim, float Range);

private:

	//给Box大小变化绑定的函数
	FOptionalSize GetBoxWidth() const;
	FOptionalSize GetBoxHeight() const;

private:
	//获取GameStyle
	const struct FSlAiGameStyle* GameStyle;
	
	TSharedPtr<class SBox> RootBox;

	float CurrentSize;

	//材质实例
	class UMaterialInstanceDynamic* PointerMaterial;

	bool IsAimed;
};
