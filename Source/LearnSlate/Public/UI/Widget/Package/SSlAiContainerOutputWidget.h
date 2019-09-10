// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SSlAiContainerBaseWidget.h"

/**
 * 
 */
class LEARNSLATE_API SSlAiContainerOutputWidget : public SSlAiContainerBaseWidget
{
public:
	SLATE_BEGIN_ARGS(SSlAiContainerOutputWidget)
	{}

	SLATE_ATTRIBUTE(int, WorkIndex)

	SLATE_END_ARGS()

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);

	//��д�������
	virtual void LeftOperate(int InputID, int InputNum, int& OutputID, int& OutputNum) override;

	//��д�Ҽ�����
	virtual void RightOperate(int InputID, int InputNum, int& OutputID, int& OutputNum) override;
};
