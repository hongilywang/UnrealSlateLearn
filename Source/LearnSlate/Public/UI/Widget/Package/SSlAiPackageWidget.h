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

	//ע�ᱳ���������¼�����playercharacter��InitPackageManagerί�н��е���
	void InitPackageManager();

private:
	//��ȡGameStyle
	const struct FSlAiGameStyle* GameStyle;

	//��������
	TSharedPtr<class SUniformGridPanel> ShortcutGrid;
	//�����ؼ�
	TSharedPtr<class SUniformGridPanel> PackageGrid;
	//�ϳɱ���
	TSharedPtr<class SUniformGridPanel> CompoundGrid;
	//�������
	TSharedPtr<class SBorder> OutputBorder;

	//���λ��
	FVector2D MousePosition;

	//DPI����
	TAttribute<float> UIScaler;

	//�Ƿ��Ѿ���ʼ������������
	bool IsInitPackageMana;
};
