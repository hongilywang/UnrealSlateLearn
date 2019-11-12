// Fill out your copyright notice in the Description page of Project Settings.


#include "SSlAiGameHUDWidget.h"
#include "SlateOptMacros.h"
#include "Engine/Engine.h"
#include "SDPIScaler.h"
#include "SOverlay.h"
#include "SSlAiShortcutWidget.h"
#include "SSlAiRayInfoWidget.h"
#include "SSlAiPointerWidget.h"
#include "SSlAiPlayerStateWidget.h"
#include "SImage.h"
#include "SSlAiGameMenuWidget.h"
#include "SSlAiChatRoomWidget.h"
#include "SSlAiPackageWidget.h"
#include "SSlAiMiniMapWidget.h"
#include "SSlAiChatShowWidget.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SSlAiGameHUDWidget::Construct(const FArguments& InArgs)
{
	UIScaler.Bind(this, &SSlAiGameHUDWidget::GetUIScaler);

	ChildSlot
	[
		SNew(SDPIScaler)
		.DPIScale(UIScaler)
		[
			SNew(SOverlay)

			//�����
			+SOverlay::Slot()
			.HAlign(HAlign_Center)
			.VAlign(VAlign_Bottom)
			[
				SAssignNew(ShortcutWidget, SSlAiShortcutWidget)
			]

			//������Ϣ
			+ SOverlay::Slot()
			.HAlign(HAlign_Center)
			.VAlign(VAlign_Top)
			[
				SAssignNew(RayInfoWidget, SSlAiRayInfoWidget)
			]

			//׼��
			+ SOverlay::Slot()
			.HAlign(HAlign_Center)
			.VAlign(VAlign_Center)
			[
				SAssignNew(PointerWidget, SSlAiPointerWidget)
			]

			//���״̬
			+SOverlay::Slot()
			.HAlign(HAlign_Left)
			.VAlign(VAlign_Top)
			[
				SAssignNew(PlayerStateWidget, SSlAiPlayerStateWidget)
			]

			//С��ͼ
			+SOverlay::Slot()
			.HAlign(HAlign_Right)
			.VAlign(VAlign_Top)
			[
				SAssignNew(MiniMapWidget, SSlAiMiniMapWidget)
			]

			//������ʾ��
			+SOverlay::Slot()
			.HAlign(HAlign_Left)
			.VAlign(VAlign_Bottom)
			.Padding(FMargin(20.f, 0.f, 0.f, 15.f))
			[
				SAssignNew(ChatShowWidget, SSlAiChatShowWidget)
			]

			//����ɫ���֣������¼��������ϷUI�м�
			+SOverlay::Slot()
			.HAlign(HAlign_Fill)
			.VAlign(VAlign_Fill)
			[
				SAssignNew(BlackShade, SBorder)
				//��ɫ͸��
				.ColorAndOpacity(TAttribute<FLinearColor>(FLinearColor(0.2f, 0.2f, 0.2f, 0.5f)))
				//��ʼ���㲻��ʾ
				.Visibility(EVisibility::Hidden)
				[
					SNew(SImage)
				]
			]
			
			//GameMenu
			+SOverlay::Slot()
			.HAlign(HAlign_Center)
			.VAlign(VAlign_Center)
			[
				SAssignNew(GameMenuWidget, SSlAiGameMenuWidget)
				.Visibility(EVisibility::Hidden)
			]

			//ChatRoom
			+SOverlay::Slot()
			.HAlign(HAlign_Left)
			.VAlign(VAlign_Bottom)
			[
				SAssignNew(ChatRoomWidget, SSlAiChatRoomWidget)
				.Visibility(EVisibility::Hidden)
			]

			//Package
			+ SOverlay::Slot()
			.HAlign(HAlign_Fill)
			.VAlign(VAlign_Fill)
			[
				SAssignNew(PackageWidget, SSlAiPackageWidget)
				//����DPI
				.UIScaler(UIScaler)
				.Visibility(EVisibility::Hidden)
			]
		]
	];
	
	InitUIMap();
}

void SSlAiGameHUDWidget::Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime)
{
	if (MessageTimeCount < 5.f)
	{
		MessageTimeCount += InDeltaTime;
	}
	else
	{
		ChatShowWidget->AddMessage(NSLOCTEXT("SlAiGame", "Enemy", "Enemy"), NSLOCTEXT("SlAiGame", "EnemyDialogue", ": Fight with me !"));
		ChatRoomWidget->AddMessage(NSLOCTEXT("SlAiGame", "Enemy", "Enemy"), NSLOCTEXT("SlAiGame", "EnemyDialogue", ": Fight with me !"));
		MessageTimeCount = 0.f;
	}
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION

float SSlAiGameHUDWidget::GetUIScaler() const
{
	return GetViewportSize().Y / 1080.f;
}

void SSlAiGameHUDWidget::ShowGameUI(EGameUIType::Type PreUI, EGameUIType::Type NextUI)
{
	//���ǰһ��ģʽ��Game
	if (PreUI == EGameUIType::Game)
		BlackShade->SetVisibility(EVisibility::Visible);
	else
		UIMap.Find(PreUI)->Get()->SetVisibility(EVisibility::Hidden);

	//�����һ��ģʽ��Game
	if (NextUI == EGameUIType::Game)
		BlackShade->SetVisibility(EVisibility::Hidden);
	else
	{
		UIMap.Find(NextUI)->Get()->SetVisibility(EVisibility::Visible);
		//��ʾ����״̬��Ӧ��UI
		if (NextUI == EGameUIType::ChatRoom)
			ChatRoomWidget->ScrollToEnd();
	}
}

FVector2D SSlAiGameHUDWidget::GetViewportSize() const
{
	FVector2D Result(1920.f, 1080.f);
	if (GEngine && GEngine->GameViewport)
		GEngine->GameViewport->GetViewportSize(Result);

	return Result;
}

void SSlAiGameHUDWidget::InitUIMap()
{
	UIMap.Add(EGameUIType::Pause, GameMenuWidget);
	UIMap.Add(EGameUIType::Package, PackageWidget);
	UIMap.Add(EGameUIType::ChatRoom, ChatRoomWidget);
	UIMap.Add(EGameUIType::Lose, GameMenuWidget);

	//ί�а�
	ChatRoomWidget->PushMessage.BindRaw(ChatShowWidget.Get(), &SSlAiChatShowWidget::AddMessage);

	MessageTimeCount = 0.f;
}
