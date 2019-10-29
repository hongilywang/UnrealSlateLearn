// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Styling/SlateWidgetStyle.h"
#include "SlateWidgetStyleContainerBase.h"
#include "SlateBrush.h"

#include "SlAiGameWidgetStyle.generated.h"

/**
 * 
 */
USTRUCT()
struct LEARNSLATE_API FSlAiGameStyle : public FSlateWidgetStyle
{
	GENERATED_USTRUCT_BODY()

	FSlAiGameStyle();
	virtual ~FSlAiGameStyle();

	// FSlateWidgetStyle
	virtual void GetResources(TArray<const FSlateBrush*>& OutBrushes) const override;
	static const FName TypeName;
	virtual const FName GetTypeName() const override { return TypeName; };
	static const FSlAiGameStyle& GetDefault();

	//ָ�������������ͼ��
	UPROPERTY(EditAnywhere, Category = Package)
		FSlateBrush NormalContainerBrush;

	//ָ����ѡ���������Brush
	UPROPERTY(EditAnywhere, Category = Package)
		FSlateBrush ChoosedContainerBrush;

	//û��ָ����ɫ��Brush
	UPROPERTY(EditAnywhere, Category = Package)
		FSlateBrush EmptyBrush;

	//�����ı�����
	UPROPERTY(EditAnywhere, Category = Package)
		FSlateBrush PackageBGBrush;

	//�ϳɱ���ͷ
	UPROPERTY(EditAnywhere, Category = Package)
		FSlateBrush CompoundArrowBrush;

	//��Ʒ��brush
	UPROPERTY(EditAnywhere, Category = Package)
		FSlateBrush ObjectBrush_1;
	UPROPERTY(EditAnywhere, Category = Package)
		FSlateBrush ObjectBrush_2;
	UPROPERTY(EditAnywhere, Category = Package)
		FSlateBrush ObjectBrush_3;
	UPROPERTY(EditAnywhere, Category = Package)
		FSlateBrush ObjectBrush_4;
	UPROPERTY(EditAnywhere, Category = Package)
		FSlateBrush ObjectBrush_5;
	UPROPERTY(EditAnywhere, Category = Package)
		FSlateBrush ObjectBrush_6;
	UPROPERTY(EditAnywhere, Category = Package)
		FSlateBrush ObjectBrush_7;

	//���߼����Ϣ��屳��
	UPROPERTY(EditAnywhere, Category = Info)
		FSlateBrush RayInfoBrush;

	//׼�ǲ���
	UPROPERTY(EditAnywhere, Category = Info)
		FSlateBrush PointerBrush;

	UPROPERTY(EditAnywhere, Category = MiniMap)
		FSlateBrush MiniMapBGBrush;

	//������Ա���ͼ
	UPROPERTY(EditAnywhere, Category = PlayerState)
		FSlateBrush PlayerStateBGBrush;

	//���ͷ�񱳾�ͼ
	UPROPERTY(EditAnywhere, Category = PlayerState)
		FSlateBrush PlayerStateHeadBGBrush;

	//Ѫ��Brush
	UPROPERTY(EditAnywhere, Category = PlayerState)
		FSlateBrush HPBrush;

	//����Brush
	UPROPERTY(EditAnywhere, Category = PlayerState)
		FSlateBrush HungerBrush;

	//���ͷ��
	UPROPERTY(EditAnywhere, Category = PlayerState)
		FSlateBrush PlayerHeadBrush;

	//60������
	UPROPERTY(EditAnywhere, Category = Common)
		FSlateFontInfo Font_60;

	//50������������
	UPROPERTY(EditAnywhere, Category = Common)
		FSlateFontInfo Font_Outline_50;

	//40������
	UPROPERTY(EditAnywhere, Category = Common)
		FSlateFontInfo Font_40;

	//40������������
	UPROPERTY(EditAnywhere, Category = Common)
		FSlateFontInfo Font_Outline_40;

	//30������
	UPROPERTY(EditAnywhere, Category = Common)
		FSlateFontInfo Font_30;

	//20������������
	UPROPERTY(EditAnywhere, Category = Common)
		FSlateFontInfo Font_Outline_20;

	//20������
	UPROPERTY(EditAnywhere, Category = Common)
		FSlateFontInfo Font_20;

	//16������������
	UPROPERTY(EditAnywhere, Category = Common)
		FSlateFontInfo Font_Outline_16;

	//16������
	UPROPERTY(EditAnywhere, Category = Common)
		FSlateFontInfo Font_16;

	//����ɫ
	UPROPERTY(EditAnywhere, Category = Common)
		FLinearColor FontColor_White;

	//����ɫ
	UPROPERTY(EditAnywhere, Category = Common)
		FLinearColor FontColor_Black;

};

/**
 */
UCLASS(hidecategories=Object, MinimalAPI)
class USlAiGameWidgetStyle : public USlateWidgetStyleContainerBase
{
	GENERATED_BODY()

public:
	/** The actual data describing the widget appearance. */
	UPROPERTY(Category=Appearance, EditAnywhere, meta=(ShowOnlyInnerProperties))
	FSlAiGameStyle WidgetStyle;

	virtual const struct FSlateWidgetStyle* const GetStyle() const override
	{
		return static_cast< const struct FSlateWidgetStyle* >( &WidgetStyle );
	}
};
