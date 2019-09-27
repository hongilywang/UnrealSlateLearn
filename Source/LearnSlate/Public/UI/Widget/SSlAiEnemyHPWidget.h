// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"

/**
 * 
 */
class LEARNSLATE_API SSlAiEnemyHPWidget : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SSlAiEnemyHPWidget)
	{}
	SLATE_END_ARGS()

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);

	void ChangeHP(float HP);
private:
	TSharedPtr<class SProgressBar> HPBar;
	//½á¹ûÑÕÉ«
	FLinearColor ResultColor;
};
