// Fill out your copyright notice in the Description page of Project Settings.


#include "SSlAiMiniMapWidget.h"
#include "SlateOptMacros.h"
#include "SlAiStyle.h"
#include "SlAiGameWidgetStyle.h"
#include "SBox.h"
#include "SImage.h"
#include "SOverlay.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Engine/TextureRenderTarget2D.h"
#include "SlateBrush.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SSlAiMiniMapWidget::Construct(const FArguments& InArgs)
{
	GameStyle = &SlAiStyle::Get().GetWidgetStyle<FSlAiGameStyle>("BPSlAiGameStyle");
	
	ChildSlot
	[
		SNew(SBox)
		.WidthOverride(320.f)
		.HeightOverride(320.f)
		[
			SNew(SOverlay)

			//背景图标
			+SOverlay::Slot()
			.HAlign(HAlign_Fill)
			.VAlign(VAlign_Fill)
			[
				SNew(SImage)
				.Image(&GameStyle->MiniMapBGBrush)
			]

			//渲染MiniMap的图片
			+SOverlay::Slot()
			.HAlign(HAlign_Center)
			.VAlign(VAlign_Center)
			[
				SAssignNew(MiniMapImage, SImage)
			]
		]
	];
	
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION

void SSlAiMiniMapWidget::RegisterMiniMap(class UTextureRenderTarget2D* MiniMapRender)
{
	//获取材质，这个是材质遮罩
	UMaterialInterface* MiniMapMatInst = LoadObject<UMaterialInterface>(nullptr, TEXT("MaterialInstanceConstant'/Game/Material/MiniMapMatInst.MiniMapMatInst'"));
	//创建动态材质
	UMaterialInstanceDynamic* MiniMapMatDynamic = UMaterialInstanceDynamic::Create(MiniMapMatInst, nullptr);
	//绑定材质属性
	MiniMapMatDynamic->SetTextureParameterValue(FName("MiniMapTex"), MiniMapRender);
	//实例化Minimap笔刷
	MiniMapBrush = new FSlateBrush();
	//设置属性
	MiniMapBrush->ImageSize = FVector2D(280.f, 280.f);
	MiniMapBrush->DrawAs = ESlateBrushDrawType::Image;
	//绑定材质资源文件
	MiniMapBrush->SetResourceObject(MiniMapMatDynamic);
	//将笔刷制作为MiniMapImage的笔刷
	MiniMapImage->SetImage(MiniMapBrush);
}

void SSlAiMiniMapWidget::UpdateMapData(const FRotator PlayerRotator, const float MiniMapSize, const TArray<FVector2D>* EnemyPosList, const TArray<bool>* EnemyLockList, const TArray<float>* EnemyRotateList)
{
	//获取Yaw, 这个Yaw从180到-180， 我们把他变成负的，然后通过角度来计算
	float YawDir = -PlayerRotator.Yaw;

	//
	NorthLocation = FVector2D(FMath::Sin(FMath::DegreesToRadians(YawDir)), FMath::Cos(FMath::DegreesToRadians(YawDir))) * 150.f + FVector2D(160.f, 160.f);
	EastLocation = FVector2D(FMath::Sin(FMath::DegreesToRadians(YawDir + 90.f)), FMath::Cos(FMath::DegreesToRadians(YawDir + 90.f))) * 150.f + FVector2D(160.f, 160.f);
	SouthLocation = FVector2D(FMath::Sin(FMath::DegreesToRadians(YawDir + 180.f)), FMath::Cos(FMath::DegreesToRadians(YawDir + 180.f))) * 150.f + FVector2D(160.f, 160.f);
	WestLocation = FVector2D(FMath::Sin(FMath::DegreesToRadians(YawDir + 270.f)), FMath::Cos(FMath::DegreesToRadians(YawDir + 270.f))) * 150.f + FVector2D(160.f, 160.f);
}

int32 SSlAiMiniMapWidget::OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const
{
	SCompoundWidget::OnPaint(Args, AllottedGeometry, MyCullingRect, OutDrawElements, LayerId, InWidgetStyle, bParentEnabled);

	//渲染玩家图片
	FSlateDrawElement::MakeBox(OutDrawElements, LayerId + 10, AllottedGeometry.ToPaintGeometry(FVector2D(155.f, 155.f), FVector2D(10.f, 10.f)), &GameStyle->PawnPointBrush, ESlateDrawEffect::None, FLinearColor(1.f, 1.f, 0.f, 1.f));

	//渲染东西南北文字
	FSlateDrawElement::MakeText(OutDrawElements, LayerId + 10, AllottedGeometry.ToPaintGeometry(NorthLocation - FVector2D(8.f, 8.f), FVector2D(16.f, 16.f)), NSLOCTEXT("SlAiGame", "N", "N"), GameStyle->Font_16, ESlateDrawEffect::None, FLinearColor(1.f, 1.f, 1.f, 1.f));
	FSlateDrawElement::MakeText(OutDrawElements, LayerId + 10, AllottedGeometry.ToPaintGeometry(SouthLocation - FVector2D(8.f, 8.f), FVector2D(16.f, 16.f)), NSLOCTEXT("SlAiGame", "S", "S"), GameStyle->Font_16, ESlateDrawEffect::None, FLinearColor(1.f, 1.f, 1.f, 1.f));
	FSlateDrawElement::MakeText(OutDrawElements, LayerId + 10, AllottedGeometry.ToPaintGeometry(EastLocation - FVector2D(8.f, 8.f), FVector2D(16.f, 16.f)), NSLOCTEXT("SlAiGame", "E", "E"), GameStyle->Font_16, ESlateDrawEffect::None, FLinearColor(1.f, 1.f, 1.f, 1.f));
	FSlateDrawElement::MakeText(OutDrawElements, LayerId + 10, AllottedGeometry.ToPaintGeometry(WestLocation - FVector2D(8.f, 8.f), FVector2D(16.f, 16.f)), NSLOCTEXT("SlAiGame", "W", "W"), GameStyle->Font_16, ESlateDrawEffect::None, FLinearColor(1.f, 1.f, 1.f, 1.f));

	return LayerId;
}
