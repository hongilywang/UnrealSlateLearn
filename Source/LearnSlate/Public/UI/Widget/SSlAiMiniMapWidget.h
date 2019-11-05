// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"

class SImage;
/**
 * 
 */
class LEARNSLATE_API SSlAiMiniMapWidget : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SSlAiMiniMapWidget)
	{}
	SLATE_END_ARGS()

		/** Constructs this widget with InArgs */
		void Construct(const FArguments& InArgs);

	virtual int32 OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const override;

	//ί�н���GameMode��������Texture��Դ
	void RegisterMiniMap(class UTextureRenderTarget2D* MiniMapRender);

	//ί�н���GameMode�������������ת���󶨵�ί����GameMode��UpdateMapData
	void UpdateMapData(const FRotator PlayerRotator, const float MiniMapSize, const TArray<FVector2D>* EnemyPosList, const TArray<bool>* EnemyLockList, const TArray<float>* EnemyRotateList);

private:
	//��ȡGameStyle
	const struct FSlAiGameStyle* GameStyle;

	//��ʾMiniMap��ͼƬ
	TSharedPtr<SImage> MiniMapImage;

	//������Ұ����
	class UMaterialInstanceDynamic* EnemyViewMatDynamic;

	struct FSlateBrush* MiniMapBrush;

	//�ĸ��������Ⱦλ��
	FVector2D NorthLocation;
	FVector2D SouthLocation;
	FVector2D EastLocation;
	FVector2D WestLocation;
};
