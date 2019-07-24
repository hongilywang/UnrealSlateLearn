// Fill out your copyright notice in the Description page of Project Settings.


#include "SlAiGameHUD.h"
#include "SSlAiGameHUDWidget.h"
#include "Engine/Engine.h"
#include "SlateBasics.h"

ASlAiGameHUD::ASlAiGameHUD()
{
	if (GEngine && GEngine->GameViewport)
	{
		SAssignNew(GameHUDWidget, SSlAiGameHUDWidget);
		GEngine->GameViewport->AddViewportWidgetContent(SNew(SWeakWidget).PossiblyNullContent(GameHUDWidget.ToSharedRef()));
	}
}
