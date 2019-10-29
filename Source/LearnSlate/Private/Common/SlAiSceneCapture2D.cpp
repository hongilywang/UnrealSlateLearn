// Fill out your copyright notice in the Description page of Project Settings.


#include "SlAiSceneCapture2D.h"
#include "Components/SceneCaptureComponent2D.h"
#include "Engine/TextureRenderTarget2D.h"

ASlAiSceneCapture2D::ASlAiSceneCapture2D()
{
	//����ÿ֡����
	GetCaptureComponent2D()->bCaptureEveryFrame = true;

	//������ȾͼƬ�ĸ�ʽ
	GetCaptureComponent2D()->CaptureSource = ESceneCaptureSource::SCS_SceneColorSceneDepth;

	//������ҵģʽΪ����ģʽ
	GetCaptureComponent2D()->ProjectionType = ECameraProjectionMode::Orthographic;

	//���ó�ʼ������Ұ��Χ
	GetCaptureComponent2D()->OrthoWidth = 3000.f;

	//������ת��ͷ����
	SetActorRotation(FRotator(-90.f, 0.f, 0.f));
}

class UTextureRenderTarget2D* ASlAiSceneCapture2D::GetMiniMapTex()
{
	//������Ⱦ��ͼ
	MiniMapTex = NewObject<UTextureRenderTarget2D>();
	//
	MiniMapTex->InitAutoFormat(256, 256);
	//�󶨵���Ⱦ�����
	GetCaptureComponent2D()->TextureTarget = MiniMapTex;
	return MiniMapTex;
}

void ASlAiSceneCapture2D::UpdateTransform(FVector NormLocation, FRotator NormRotator)
{
	//����λ��Ϊ����Ϸ�1000
	SetActorLocation(NormLocation + FVector(0.f, 0.f, 1000.f));
	//������תΪ�����ת
	SetActorRotation(FRotator(-90.f, NormRotator.Yaw, NormRotator.Roll));
}
