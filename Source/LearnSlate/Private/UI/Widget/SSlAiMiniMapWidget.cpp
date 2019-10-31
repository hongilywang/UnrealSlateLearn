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

}
