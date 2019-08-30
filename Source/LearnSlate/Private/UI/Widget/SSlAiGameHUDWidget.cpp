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
		]
	];
	
}
END_SLATE_FUNCTION_BUILD_OPTIMIZATION

float SSlAiGameHUDWidget::GetUIScaler() const
{
	return GetViewportSize().Y / 1080.f;
}

FVector2D SSlAiGameHUDWidget::GetViewportSize() const
{
	FVector2D Result(1920.f, 1080.f);
	if (GEngine && GEngine->GameViewport)
		GEngine->GameViewport->GetViewportSize(Result);

	return Result;
}
