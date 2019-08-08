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

	//�󶨵�UIScaler�ķ���
	float GetUIScaler() const;

public:

	//�����ָ��
	TSharedPtr<class SSlAiShortcutWidget> ShortcutWidget;
	//������Ϣ��
	TSharedPtr<class SSlAiRayInfoWidget> RayInfoWidget;

private:
	//��ȡ��Ļsize
	FVector2D GetViewportSize() const;


private:

	//DPI����
	TAttribute<float> UIScaler;
};
