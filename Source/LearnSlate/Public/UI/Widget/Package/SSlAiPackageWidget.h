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

};
