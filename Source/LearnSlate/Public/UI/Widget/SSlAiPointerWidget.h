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

	//��PlayerController�󶨵��¼����޸�׼���Ƿ������Ѿ����ؽ���
	void UpdatePointer(bool IsAim, float Range);

private:

	//��Box��С�仯�󶨵ĺ���
	FOptionalSize GetBoxWidth() const;
	FOptionalSize GetBoxHeight() const;

private:
	//��ȡGameStyle
	const struct FSlAiGameStyle* GameStyle;
	
	TSharedPtr<class SBox> RootBox;

	float CurrentSize;

	//����ʵ��
	class UMaterialInstanceDynamic* PointerMaterial;

	bool IsAimed;
};
