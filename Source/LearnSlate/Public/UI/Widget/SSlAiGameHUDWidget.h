// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"
#include "SlAiTypes.h"

/**
 * 
 */
class LEARNSLATE_API SSlAiGameHUDWidget : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SSlAiGameHUDWidget)
	{}
	SLATE_END_ARGS()

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);

	//�󶨵�UIScaler�ķ���
	float GetUIScaler() const;

	//��ʾ��ϷUI����PlayerController��ShowGameUIί�а�
	void ShowGameUI(EGameUIType::Type PreUI, EGameUIType::Type NextUI);

public:

	//�����ָ��
	TSharedPtr<class SSlAiShortcutWidget> ShortcutWidget;
	//������Ϣ��
	TSharedPtr<class SSlAiRayInfoWidget> RayInfoWidget;
	//׼��
	TSharedPtr<class SSlAiPointerWidget> PointerWidget;
	//���״ָ̬��
	TSharedPtr<class SSlAiPlayerStateWidget> PlayerStateWidget;
	//��Ϸ�˵�
	TSharedPtr<class SSlAiGameMenuWidget> GameMenuWidget;
	//������
	TSharedPtr<class SSlAiChatRoomWidget> ChatRoomWidget;
	//����
	TSharedPtr<class SSlAiPackageWidget> PackageWidget;

private:
	//��ȡ��Ļsize
	FVector2D GetViewportSize() const;

	//��UI�󶨵�UIMap
	void InitUIMap();

private:

	//DPI����
	TAttribute<float> UIScaler;

	//����
	TSharedPtr<class SBorder> BlackShade;

	//UIMap
	TMap<EGameUIType::Type, TSharedPtr<SCompoundWidget>> UIMap;
};
