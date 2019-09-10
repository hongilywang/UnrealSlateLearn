// Fill out your copyright notice in the Description page of Project Settings.


#include "SSlAiContainerOutputWidget.h"
#include "SlateOptMacros.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SSlAiContainerOutputWidget::Construct(const FArguments& InArgs)
{
	SSlAiContainerBaseWidget::Construct(
		SSlAiContainerBaseWidget::FArguments().WorkIndex(InArgs._WorkIndex)
	);
}
END_SLATE_FUNCTION_BUILD_OPTIMIZATION

void SSlAiContainerOutputWidget::LeftOperate(int InputID, int InputNum, int& OutputID, int& OutputNum)
{
	//���������ƷΪ�գ�����ʲô�����ʲô
	if (ObjectIndex == 0)
	{
		OutputID = InputID;
		OutputNum = InputNum;
		return;
	}

	//���������Ʒ�뱾����Ʒ��ͬ���ҿ��Ե���
	if (InputID == ObjectIndex && MultiplyAble(InputID))
	{
		OutputID = ObjectIndex;
		OutputNum = (InputNum + ObjectNum <= 64) ? (InputNum + ObjectNum) : 64;
		ObjectNum = (InputNum + ObjectNum <= 64) ? 0 : (InputNum + ObjectNum - 64);
		//ִ�кϳ����ί��
		CompoundOutput.ExecuteIfBound(ObjectIndex, OutputNum - InputNum);
		return;
	}

	//�����Ʒ����ͬ��������Ʒ��ͬ���ǲ��ܵ��ӣ������Ʒ������������Ʒ
	OutputID = ObjectIndex;
	OutputNum = ObjectNum;
}

void SSlAiContainerOutputWidget::RightOperate(int InputID, int InputNum, int& OutputID, int& OutputNum)
{

}
