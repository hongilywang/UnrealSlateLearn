// Fill out your copyright notice in the Description page of Project Settings.


#include "SlAiGameHUD.h"
#include "Engine/Engine.h"
#include "SlateBasics.h"
#include "Kismet/GameplayStatics.h"

#include "SSlAiGameHUDWidget.h"
#include "SSlAiShortcutWidget.h"
#include "SSlAiRayInfoWidget.h"
#include "SSlAiPointerWidget.h"
#include "SSlAiPlayerStateWidget.h"
#include "SSlAiPackageWidget.h"
#include "SSlAiMiniMapWidget.h"
#include "SSlAiGameMenuWidget.h"

#include "SlAiPlayerController.h"
#include "SlAiPlayerState.h"
#include "SlAiGameMode.h"

ASlAiGameHUD::ASlAiGameHUD()
{
	if (GEngine && GEngine->GameViewport)
	{
		SAssignNew(GameHUDWidget, SSlAiGameHUDWidget);
		GEngine->GameViewport->AddViewportWidgetContent(SNew(SWeakWidget).PossiblyNullContent(GameHUDWidget.ToSharedRef()));
	}
}

void ASlAiGameHUD::BeginPlay()
{
	Super::BeginPlay();

	GM = Cast<ASlAiGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	if (!GM)
		return;

	//先确保要调用
	GM->InitGamePlayModule();
	//绑定注册快捷栏容器
	GameHUDWidget->ShortcutWidget->RegisterShortcutContainer.BindUObject(GM->SPState, &ASlAiPlayerState::RegisterShortcutContainer);
	//绑定RayInfo
	GameHUDWidget->RayInfoWidget->RegisterRayInfoEvent.BindUObject(GM->SPState, &ASlAiPlayerState::RegisterRayInfoEvent);
	//绑定修改准星委托
	GM->SPController->UpdatePointer.BindRaw(GameHUDWidget->PointerWidget.Get(), &SSlAiPointerWidget::UpdatePointer);
	//绑定更新玩家状态的委托
	GM->SPState->UpdateStateWidget.BindRaw(GameHUDWidget->PlayerStateWidget.Get(), &SSlAiPlayerStateWidget::UpdateStateWidget);
	//绑定显示UI委托
	GM->SPController->ShowGameUI.BindRaw(GameHUDWidget.Get(), &SSlAiGameHUDWidget::ShowGameUI);
	//初始化背包管理
	GM->InitPackageManager.BindRaw(GameHUDWidget->PackageWidget.Get(), &SSlAiPackageWidget::InitPackageManager);
	//绑定注册小地图贴图委托
	GM->RegisterMiniMap.BindRaw(GameHUDWidget->MiniMapWidget.Get(), &SSlAiMiniMapWidget::RegisterMiniMap);
	//绑定更新小地图数据委托
	GM->UpdateMapData.BindRaw(GameHUDWidget->MiniMapWidget.Get(), &SSlAiMiniMapWidget::UpdateMapData);
	//保存游戏事件绑定
	GameHUDWidget->GameMenuWidget->SaveGameDele.BindUObject(GM, &ASlAiGameMode::SaveGame);
}

