// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SlAiTypes.h"
#include "Widgets/SCompoundWidget.h"

class SBox;
class STextBlock;
class SVerticalBox;
struct MenuGroup;
class SSlAiGameOptionWidget;
class SSlAiNewGameWidget;
class SSlAiChooseRecordWidget;
/**
 * 
 */
class LEARNSLATE_API SSlAiMenuWidget : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SSlAiMenuWidget)
	{}
	SLATE_END_ARGS()

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);

	//��дtick����
	virtual void Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime) override;

private:
	//�󶨵�����MenuItem�ķ���
	void MenuItemOnClicked(EMenuItem::Type ItemType);
	//�޸�����
	void ChangeCulture(ECultureTeam Culture);
	//�޸�����
	void ChangeVolume(const float MusicVolume, const float SoundVolume);
	//��ʼ�����еĿؼ�
	void InitializeMenuList();
	//ѡ����ʾ�Ľ���
	void ChooseWidget(EMenuType::Type WidgetType);
	//�޸Ĳ˵���С
	void ResetWidgetSize(float NewWidth, float NewHeight);
	//��ʼ���������
	void InitializedAnimator();
	//���Źرն���
	void PlayClose(EMenuType::Type NewMenu);

private:
	//������ڵ�
	TSharedPtr<SBox> RootSizeBox;

	//��ȡMenuStyle
	const struct FSlAiMenuStyle* MenuStyle;

	//�������
	TSharedPtr<STextBlock> TitleText;

	//��ֱ�б�
	TSharedPtr<SVerticalBox> ContentBox;

	//����˵���
	TMap<EMenuType::Type, TSharedPtr<MenuGroup>> MenuMap;

	//��Ϸ����widget��ָ��
	TSharedPtr<SSlAiGameOptionWidget> GameOptionWidget;
	//����Ϸ�ؼ�ָ��
	TSharedPtr<SSlAiNewGameWidget> NewGameWidget;
	//ѡ��浵�ؼ�ָ��
	TSharedPtr<SSlAiChooseRecordWidget> ChooseRecordWidget;

	//����������
	FCurveSequence MenuAimation;
	//���߿�����
	FCurveHandle MenuCurve;
	//���������µĸ߶�
	float CureentHeight;
	//�Ƿ��Ѿ���ʾ��Menu���
	bool IsMenuShow;
	//�Ƿ���ס��ť
	bool ControlLocked;
	//���浱ǰ�Ķ���״̬
	EMenuAnim::Type AnimState;
	//���浱ǰ�Ĳ˵�
	EMenuType::Type CurrentMenu;
};
