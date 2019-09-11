// Fill out your copyright notice in the Description page of Project Settings.


#include "SSlAiContainerShortcutWidget.h"
#include "SlateOptMacros.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SSlAiContainerShortcutWidget::Construct(const FArguments& InArgs)
{
	SSlAiContainerBaseWidget::Construct(
		SSlAiContainerBaseWidget::FArguments().WorkIndex(InArgs._WorkIndex)
	);
}
END_SLATE_FUNCTION_BUILD_OPTIMIZATION

void SSlAiContainerShortcutWidget::ResetContainerPara(int ObjectID, int Num)
{
	//�Ƿ�ı�
	bool IsChanged = false;
	if (ObjectIndex != ObjectID || ObjectNum != Num)
		IsChanged = true;

	//���ø����¼�
	SSlAiContainerBaseWidget::ResetContainerPara(ObjectID, Num);

	//����иı䣬ִ�п�����޸ĸ���ί�У��������������Լ��µ���ƷID
	if (IsChanged)
		PackShortChange.ExecuteIfBound(WorkIndex.Get(), ObjectID, Num);
}
