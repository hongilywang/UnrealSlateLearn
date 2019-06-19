// Fill out your copyright notice in the Description page of Project Settings.

#include "LearnSlate.h"
#include "Modules/ModuleManager.h"
#include "SlAiStyle.h"

IMPLEMENT_PRIMARY_GAME_MODULE( FDefaultGameModuleImpl, LearnSlate, "LearnSlate" );

void FLearnSlateModule::StartupModule()
{
	//��ʼ����ʽ
	FSlateStyleRegistry::UnRegisterSlateStyle(SlAiStyle::GetStyleSetName());
	SlAiStyle::Initialize();
}

void FLearnSlateModule::ShutdownModule()
{
	SlAiStyle::ShutDown();
}
