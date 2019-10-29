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

	//ί�н���GameMode��������Texture��Դ
	void RegisterMiniMap(class UTextureRenderTarget2D* MiniMapRender);

private:
	//��ȡGameStyle
	const struct FSlAiGameStyle* GameStyle;

	//��ʾMiniMap��ͼƬ
	TSharedPtr<SImage> MiniMapImage;

	//������Ұ����
	class UMaterialInstanceDynamic* EnemyViewMatDynamic;

	struct FSlateBrush* MiniMapBrush;
};
