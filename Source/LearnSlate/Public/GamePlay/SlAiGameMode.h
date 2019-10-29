// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "SlAiGameMode.generated.h"

//��ʼ������������ί��
DECLARE_DELEGATE(FInitPackageManager)
//ע��MiniMap����ͼ�Ͳ���
DECLARE_DELEGATE_OneParam(FRegisterMiniMap, class UTextureRenderTarget2D*)
/**
 * 
 */
UCLASS()
class LEARNSLATE_API ASlAiGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	ASlAiGameMode();

	//��д
	virtual void Tick(float DeltaSeconds) override;

	//�����ֵ�� ��GameHUD���ã�����������������
	void InitGamePlayModule();

public:
	class ASlAiPlayerController* SPController;
	class ASlAiPlayerCharacter* SPCharacter;
	class ASlAiPlayerState* SPState;
	//��ʼ����������ί�У��󶨵ķ�ʽPackageWidget��InitPackageManager����
	FInitPackageManager InitPackageManager;

	//����ί�У��󶨵ķ�����MiniMapWidget��RegisterMiniMap
	FRegisterMiniMap RegisterMiniMap;

protected:
	virtual void BeginPlay() override;

	void InitializePackage();

	//��ʼ�������С��ͼ�����
	void InitializeMiniMapCamera();

private:
	//�Ƿ��Ѿ���ʼ������
	bool IsInitPackage;

	//�Ƿ��Ѿ�����С��ͼ
	bool IsCreateMiniMap;

	//��Ⱦ���ָ��
	class ASlAiSceneCapture2D* MiniMapCamera;
};
