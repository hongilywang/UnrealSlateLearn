// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class LEARNSLATE_API SlAiInternation
{
public:
	static void Register(FText value)
	{
		return;
	}
};

#define LOCTEXT_NAMESPACE "SlAiMenu"


SlAiInternation::Register(LOCTEXT("Menu", "Menu")); //�˵�
SlAiInternation::Register(LOCTEXT("StartGame", "StartGame")); //��ʼ��Ϸ
SlAiInternation::Register(LOCTEXT("GameOption", "GameOption")); //��Ϸ����
SlAiInternation::Register(LOCTEXT("QuitGame", "QuitGame")); //�˳���Ϸ

SlAiInternation::Register(LOCTEXT("NewGame", "NewGame")); //����Ϸ
SlAiInternation::Register(LOCTEXT("LoadRecord", "LoadRecord")); //���ش浵

SlAiInternation::Register(LOCTEXT("ChooseRecord", "ChooseRecord")); //ѡ��浵

SlAiInternation::Register(LOCTEXT("RecordName", "RecordName")); //�浵��
SlAiInternation::Register(LOCTEXT("EnterGame", "EnterGame")); //������Ϸ
SlAiInternation::Register(LOCTEXT("EnterRecord", "EnterRecord")); //����浵
SlAiInternation::Register(LOCTEXT("RecordNameHint", "Input Record Name!")); //����浵��
SlAiInternation::Register(LOCTEXT("NameRepeatedHint", "Record Name Repeated")); //����浵��

SlAiInternation::Register(LOCTEXT("Chinese", "Chinese")); //����
SlAiInternation::Register(LOCTEXT("English", "English")); //Ӣ��
SlAiInternation::Register(LOCTEXT("Music", "Music")); //����
SlAiInternation::Register(LOCTEXT("Sound", "Sound")); //��Ч

SlAiInternation::Register(LOCTEXT("GoBack", "GoBack")); //����

#undef LOCTEXT_NAMESPACE

#define LOCTEXT_NAMESPACE "SlAiGame"


SlAiInternation::Register(LOCTEXT("E", "E"));//��
SlAiInternation::Register(LOCTEXT("S", "S"));//��
SlAiInternation::Register(LOCTEXT("W", "W"));//��
SlAiInternation::Register(LOCTEXT("N", "N"));//��

SlAiInternation::Register(LOCTEXT("Player", "Player"));//���
SlAiInternation::Register(LOCTEXT("Enemy", "Enemy"));//����

SlAiInternation::Register(LOCTEXT("EnemyDialogue", ": Fight with me !"));//���˵ĶԻ�
SlAiInternation::Register(LOCTEXT("Send", "Send"));//����

SlAiInternation::Register(LOCTEXT("GameOption", "GameOption"));//��Ϸ����
SlAiInternation::Register(LOCTEXT("SaveGame", "SaveGame"));//������Ϸ
SlAiInternation::Register(LOCTEXT("SaveCompleted", "SaveCompleted"));//�������
SlAiInternation::Register(LOCTEXT("QuitGame", "QuitGame"));//�˳���Ϸ
SlAiInternation::Register(LOCTEXT("GoBack", "GoBack"));//����


#undef LOCTEXT_NAMESPACE