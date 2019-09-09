// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"
#include "SlAiTypes.h"

/**
 * 
 */
class LEARNSLATE_API SSlAiContainerBaseWidget : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SSlAiContainerBaseWidget)
	{}

	SLATE_ATTRIBUTE(int, WorkIndex)

	SLATE_END_ARGS()

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);

	//��ȡʵ��
	static TSharedPtr<SSlAiContainerBaseWidget> CreateContainer(EContainerType::Type NeedType, int WorkID);

	//����ǲ��ƶ��������״̬
	void UpdateHovered(bool IsHovered);

	//������������
	virtual void ResetContainerPara(int ObjectID, int Num);

	//��ȡ������ƷID������
	int GetIndex() const;
	int GetNum() const;

	//����������
	virtual void LeftOperate(int InputID, int InputNum, int& OutputID, int& OutputNum);

	//�Ҽ��������
	virtual void RightOperate(int InputID, int InputNum, int& OutputID, int& OutputNum);

protected:
	//��ȡ�Ƿ���Ե���
	bool MultiplyAble(int ObjectID);

protected:

	//�����������
	TSharedPtr<class SBorder> ContainerBorder;
	TSharedPtr<class SBorder> ObjectImage;
	TSharedPtr<class STextBlock> ObjectNumText;

	//��ȡGameStyle
	const struct FSlAiGameStyle* GameStyle;

	//������ţ����ڱ궨�ڱ����е�λ������
	TAttribute<int> WorkIndex;

	//�Ƿ������Hover״̬
	bool IsHover;

	//��Ʒ���
	int ObjectIndex;
	//��Ʒ����
	int ObjectNum;
};
