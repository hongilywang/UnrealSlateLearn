// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"

/**
 * 
 */
class LEARNSLATE_API SSlAiPlayerStateWidget : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SSlAiPlayerStateWidget)
	{}
	SLATE_END_ARGS()

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);

	//����״̬�¼�
	void UpdateStateWidget(float HPValue, float HungerValue);

private:
	//��ȡGameStyle
	const struct FSlAiGameStyle* GameStyle;

	//Ѫ��
	TSharedPtr<class SProgressBar> HPBar;

	//������
	TSharedPtr<class SProgressBar> HungerBar;
};
