// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"
#include "SlAiTypes.h"

//�浵ί��
DECLARE_DELEGATE(FSaveGameDelegate)
/**
 * 
 */
class LEARNSLATE_API SSlAiGameMenuWidget : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SSlAiGameMenuWidget)
	{}
	SLATE_END_ARGS()

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);

	//
	void GameLose();
	//���ò˵�
	void ResetMenu();

public:
	//�浵ί�У� ��GameMode��SaveGame����
	FSaveGameDelegate SaveGameDele;

private: 
	void InitializeWidget();

	FReply OptionEvent();
	FReply SaveGameEvent();
	FReply QuitGameEvent();
	FReply GoBackEvent();

	//�޸�����
	void ChangeCulture(ECultureTeam Culture);
	//�޸�����
	void ChangeVolume(const float MusicVolume, const float SoundVolume);

private:

	TSharedPtr<class SBox> RootBox;
	TSharedPtr<class SVerticalBox> VertBox;
	TSharedPtr<class SButton> SaveGameButton;
	TSharedPtr<class STextBlock> SaveGameText;
	TSharedPtr<class SButton> QuitGameButton;

	TArray<TSharedPtr<SCompoundWidget>> MenuItemList;
	TArray<TSharedPtr<SCompoundWidget>> OptionItemList;

	const struct FSlAiGameStyle* GameStyle;
};
