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

			//快捷栏
			+SOverlay::Slot()
			.HAlign(HAlign_Center)
			.VAlign(VAlign_Bottom)
			[
				SAssignNew(ShortcutWidget, SSlAiShortcutWidget)
			]

			//射线信息
			+ SOverlay::Slot()
			.HAlign(HAlign_Center)
			.VAlign(VAlign_Top)
			[
				SAssignNew(RayInfoWidget, SSlAiRayInfoWidget)
			]

			//准星
			+ SOverlay::Slot()
			.HAlign(HAlign_Center)
			.VAlign(VAlign_Center)
			[
				SAssignNew(PointerWidget, SSlAiPointerWidget)
			]

			//玩家状态
			+SOverlay::Slot()
			.HAlign(HAlign_Left)
			.VAlign(VAlign_Top)
			[
				SAssignNew(PlayerStateWidget, SSlAiPlayerStateWidget)
			]

			//小地图
			+SOverlay::Slot()
			.HAlign(HAlign_Right)
			.VAlign(VAlign_Top)
			[
				SAssignNew(MiniMapWidget, SSlAiMiniMapWidget)
			]

			//聊天显示栏
			+SOverlay::Slot()
			.HAlign(HAlign_Left)
			.VAlign(VAlign_Bottom)
			.Padding(FMargin(20.f, 0.f, 0.f, 15.f))
			[
				SAssignNew(ChatShowWidget, SSlAiChatShowWidget)
			]

			//暗黑色遮罩，放在事件界面和游戏UI中间
			+SOverlay::Slot()
			.HAlign(HAlign_Fill)
			.VAlign(VAlign_Fill)
			[
				SAssignNew(BlackShade, SBorder)
				//黑色透明
				.ColorAndOpacity(TAttribute<FLinearColor>(FLinearColor(0.2f, 0.2f, 0.2f, 0.5f)))
				//开始涉足不显示
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
				//设置DPI
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
	//如果前一个模式是Game
	if (PreUI == EGameUIType::Game)
		BlackShade->SetVisibility(EVisibility::Visible);
	else
		UIMap.Find(PreUI)->Get()->SetVisibility(EVisibility::Hidden);

	//如果下一个模式是Game
	if (NextUI == EGameUIType::Game)
		BlackShade->SetVisibility(EVisibility::Hidden);
	else
	{
		UIMap.Find(NextUI)->Get()->SetVisibility(EVisibility::Visible);
		//显示现在状态对应的UI
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

	//委托绑定
	ChatRoomWidget->PushMessage.BindRaw(ChatShowWidget.Get(), &SSlAiChatShowWidget::AddMessage);

	MessageTimeCount = 0.f;
}
