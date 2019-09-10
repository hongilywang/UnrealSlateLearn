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
	//如果本地物品为空，输入什么就输出什么
	if (ObjectIndex == 0)
	{
		OutputID = InputID;
		OutputNum = InputNum;
		return;
	}

	//如果输入物品与本地物品相同并且可以叠加
	if (InputID == ObjectIndex && MultiplyAble(InputID))
	{
		OutputID = ObjectIndex;
		OutputNum = (InputNum + ObjectNum <= 64) ? (InputNum + ObjectNum) : 64;
		ObjectNum = (InputNum + ObjectNum <= 64) ? 0 : (InputNum + ObjectNum - 64);
		//执行合成输出委托
		CompoundOutput.ExecuteIfBound(ObjectIndex, OutputNum - InputNum);
		return;
	}

	//如果物品不相同，或者物品相同但是不能叠加，输出物品，丢弃输入物品
	OutputID = ObjectIndex;
	OutputNum = ObjectNum;
}

void SSlAiContainerOutputWidget::RightOperate(int InputID, int InputNum, int& OutputID, int& OutputNum)
{

}
