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

			//����ͼ��
			+SOverlay::Slot()
			.HAlign(HAlign_Fill)
			.VAlign(VAlign_Fill)
			[
				SNew(SImage)
				.Image(&GameStyle->MiniMapBGBrush)
			]

			//��ȾMiniMap��ͼƬ
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
	//��ȡ���ʣ�����ǲ�������
	UMaterialInterface* MiniMapMatInst = LoadObject<UMaterialInterface>(nullptr, TEXT("MaterialInstanceConstant'/Game/Material/MiniMapMatInst.MiniMapMatInst'"));
	//������̬����
	UMaterialInstanceDynamic* MiniMapMatDynamic = UMaterialInstanceDynamic::Create(MiniMapMatInst, nullptr);
	//�󶨲�������
	MiniMapMatDynamic->SetTextureParameterValue(FName("MiniMapTex"), MiniMapRender);
	//ʵ����Minimap��ˢ
	MiniMapBrush = new FSlateBrush();
	//��������
	MiniMapBrush->ImageSize = FVector2D(280.f, 280.f);
	MiniMapBrush->DrawAs = ESlateBrushDrawType::Image;
	//�󶨲�����Դ�ļ�
	MiniMapBrush->SetResourceObject(MiniMapMatDynamic);
	//����ˢ����ΪMiniMapImage�ı�ˢ
	MiniMapImage->SetImage(MiniMapBrush);
}

void SSlAiMiniMapWidget::UpdateMapData(const FRotator PlayerRotator, const float MiniMapSize, const TArray<FVector2D>* EnemyPosList, const TArray<bool>* EnemyLockList, const TArray<float>* EnemyRotateList)
{
	//��ȡYaw, ���Yaw��180��-180�� ���ǰ�����ɸ��ģ�Ȼ��ͨ���Ƕ�������
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

	//��Ⱦ���ͼƬ
	FSlateDrawElement::MakeBox(OutDrawElements, LayerId + 10, AllottedGeometry.ToPaintGeometry(FVector2D(155.f, 155.f), FVector2D(10.f, 10.f)), &GameStyle->PawnPointBrush, ESlateDrawEffect::None, FLinearColor(1.f, 1.f, 0.f, 1.f));

	//��Ⱦ�����ϱ�����
	FSlateDrawElement::MakeText(OutDrawElements, LayerId + 10, AllottedGeometry.ToPaintGeometry(NorthLocation - FVector2D(8.f, 8.f), FVector2D(16.f, 16.f)), NSLOCTEXT("SlAiGame", "N", "N"), GameStyle->Font_16, ESlateDrawEffect::None, FLinearColor(1.f, 1.f, 1.f, 1.f));
	FSlateDrawElement::MakeText(OutDrawElements, LayerId + 10, AllottedGeometry.ToPaintGeometry(SouthLocation - FVector2D(8.f, 8.f), FVector2D(16.f, 16.f)), NSLOCTEXT("SlAiGame", "S", "S"), GameStyle->Font_16, ESlateDrawEffect::None, FLinearColor(1.f, 1.f, 1.f, 1.f));
	FSlateDrawElement::MakeText(OutDrawElements, LayerId + 10, AllottedGeometry.ToPaintGeometry(EastLocation - FVector2D(8.f, 8.f), FVector2D(16.f, 16.f)), NSLOCTEXT("SlAiGame", "E", "E"), GameStyle->Font_16, ESlateDrawEffect::None, FLinearColor(1.f, 1.f, 1.f, 1.f));
	FSlateDrawElement::MakeText(OutDrawElements, LayerId + 10, AllottedGeometry.ToPaintGeometry(WestLocation - FVector2D(8.f, 8.f), FVector2D(16.f, 16.f)), NSLOCTEXT("SlAiGame", "W", "W"), GameStyle->Font_16, ESlateDrawEffect::None, FLinearColor(1.f, 1.f, 1.f, 1.f));

	return LayerId;
}
