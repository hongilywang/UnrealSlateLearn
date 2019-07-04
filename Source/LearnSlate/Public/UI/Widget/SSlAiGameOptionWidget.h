// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SlAiTypes.h"
#include "Widgets/SCompoundWidget.h"

class SCheckBox;
class SSlider;
class STextBlock;

//�޸���Ӣ��ί��
DECLARE_DELEGATE_OneParam(FChangeCulture, const ECultureTeam)
//�޸�����ί��
DECLARE_DELEGATE_TwoParams(FChangeVolume, const float, const float)

/**
 * 
 */
class LEARNSLATE_API SSlAiGameOptionWidget : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SSlAiGameOptionWidget)
	{}
	SLATE_EVENT(FChangeCulture, ChangeCulture)
	SLATE_EVENT(FChangeVolume, ChangeVolume)
	SLATE_END_ARGS()

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);

private:

	//ͳһ������ʽ
	void StyleInitialize();

	//����checkbox�¼�
	void ZhCheckBoxStateChanged(ECheckBoxState NewState);
	//Ӣ��checkbox�¼�
	void EnCheckBoxStateChanged(ECheckBoxState NewState);
	//�����仯�¼�
	void MusicSliderChanged(float value);
	//��Ч�仯�¼�
	void SoundSliderChanged(float value);

private:
	const struct FSlAiMenuStyle* MenuStyle;
	//��ȡCheckBoxָ��
	TSharedPtr<SCheckBox> EnCheckBox;
	TSharedPtr<SCheckBox> ZhCheckBox;
	//����������
	TSharedPtr<SSlider> MuSlider; //��������
	TSharedPtr<SSlider> SoSlider; //��Ч
	//�������ٷְ�
	TSharedPtr<STextBlock> MuTexBlock;
	//�������ٷְ�
	TSharedPtr<STextBlock> SoTexBlock;

	//ί�б���
	FChangeCulture ChangeCulture;
	FChangeVolume ChangeVolume;
};
