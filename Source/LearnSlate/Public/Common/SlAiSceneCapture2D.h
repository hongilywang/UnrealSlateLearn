// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/SceneCapture2D.h"
#include "SlAiSceneCapture2D.generated.h"

/**
 * 
 */
UCLASS()
class LEARNSLATE_API ASlAiSceneCapture2D : public ASceneCapture2D
{
	GENERATED_BODY()
	
public:
	ASlAiSceneCapture2D();

	//��ȡMiniMapText
	class UTextureRenderTarget2D* GetMiniMapTex();

	//�����������λ�úͳ���
	void UpdateTransform(FVector NormLocation, FRotator NormRotator);

	void UpdateMiniMapWidth(int Delta);

private:
	class UTextureRenderTarget2D* MiniMapTex;
};